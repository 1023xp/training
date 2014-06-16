#
# (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license.
#
use strict;
use warnings;
use Range;

# ------------------------------------------------------------
# Examine the contents of an ELF file. Allow the contents of a
# particular page to be examined.
package Elf;

# Static methods and constants
use constant PAGE_SIZE_SHIFT => 12;
use constant PAGE_SIZE => (1 << PAGE_SIZE_SHIFT);
sub page_align_down
{
    my $addr = shift;
    return $addr & ~(PAGE_SIZE-1);
}

sub page_align_up
{
    my $addr = shift;
    return page_align_down($addr + PAGE_SIZE - 1);
}





sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = {};
    $s->{_file} = shift;
    $s->{_suppress_warn_on_bad_magic} = shift; # undef is false...
    bless $s, $c;
    unless ($s->file) {
	warn("No file specified");
	return undef;
    }
    return undef unless $s->_open_file;
    return undef unless $s->_load_file_header;
    return undef unless $s->_load_segments;
#    return undef unless $s->_load_sections;
#    return undef unless $s->_correlate;
    return $s;
}

use constant ROOT_UID => 0;

sub _open_file
{
    my $s = shift;

    # If we're root, we may be unable to read the file unless we
    # temporarily become its owner (since it may be on an NFS
    # partition with root_squash). We can revert to our own uid
    # immediately after the open. Sadly, the perl -r and -R tests will
    # suggest that it *is* readable, so we'll simply always shift uid
    # if we are root.

    my $old_euid; # If undef, we haven't shifted uid
    if ($< == ROOT_UID) {
	$old_euid = $>;
	my @statinfo = stat($s->{_file});
	# Change uid to file owner
	$! = 0;
	$> = $statinfo[4];
	warn("Failed to change userid to $statinfo[4] : $!") if $!;
    }

    my $fh;
    unless(open($fh, "< $s->{_file}")) {
	warn("Can't open file $s->{_file} for reading : $!");
	$fh = undef;
    }

    if (defined $old_euid) {
	$! = 0;
	$> = $old_euid;
	warn("Failed to revert userid to $old_euid : $!") if $!;
    }

    $s->{_fh} = $fh if $fh;
    return defined $fh;
}

sub DESTROY
{
    my $s = shift;
    if ($s->{_fh}) {
	close $s->{_fh};
	$s->{_fh} = undef;
    }
}

sub _lazy_load_sections
{
    my $s = shift;
    return 1 if $s->{_lazy_load_sections};
    $s->{_lazy_load_sections} = 1; # Stop possible recursion...
    return undef unless $s->_load_sections;
    return undef unless $s->_correlate;
    return 1;
}

sub _correlate
{
    my $s = shift;

    my @sections = $s->sections;
    my $string_section_index = $s->{_elf_header}->{shstrnxdx};
    unless ($string_section_index <= scalar(@sections)) {
	warn("Invalid string section index : $string_section_index");
	return undef;
    }
    $s->{_string_section} = $s->section($string_section_index);

    unless ($s->{_string_section}->is_string_table) {
	warn("Invalid string section: $string_section_index");
	return undef;
    }

    foreach my $section (@sections) {
	$section->set_string_section($s->{_string_section});
	$section->set_fh($s->{_fh});
	if ($section->is_symbol_table) {
	    $s->{_symbol_table_section} = $section;
	    my $symbol_string_table = $s->section($section->link);
	    # Now load on demand
	    $section->load_symbols($symbol_string_table);
	}
    }
    return 1;
}


sub find_symbols_in_mem_range
{
    my $s = shift;
    my $r = shift;

    $s->_lazy_load_sections;

    my $symtab = $s->{_symbol_table_section};
    return () unless $symtab;
    
    return $symtab->find_symbols_in_mem_range($r);
}

sub defined_symbols
{
    my $s = shift;
    return grep { $_->is_defined } $s->all_symbols;
}

sub all_symbols
{
    my $s = shift;

    $s->_lazy_load_sections;

    my $symtab = $s->{_symbol_table_section};
    return () unless $symtab;
    return $symtab->symbols;
}

sub symbol_by_name
{
    my $s = shift;
    my $name = shift;
    foreach my $sym ($s->all_symbols) {
	return $sym if $sym->name eq $name;
    }
    return undef;
}

sub symbols_in_section
{
    my $s = shift;

    $s->_lazy_load_sections;

    my $section = shift;
    return $s->find_symbols_in_mem_range($section->mem_range);
}

sub file { return $_[0]->{_file}; }

=pod

    The ELF header format (from /usr/include/elf.h)
			   
#define EI_NIDENT (16)
    
typedef struct {
	
    unsigned char	e_ident[NIDENT];	/* Magic number and other info */
    Elf32_Half	e_type;			/* Object file type */
    Elf32_Half	e_machine;		/* Architecture */
    Elf32_Word	e_version;		/* Object file version */
    Elf32_Addr	e_entry;		/* Entry point virtual address */
    Elf32_Off	e_phoff;		/* Program header table file offset */
    Elf32_Off	e_shoff;		/* Section header table file offset */
    Elf32_Word	e_flags;		/* Processor-specific flags */
    Elf32_Half	e_ehsize;		/* ELF header size in bytes */
    Elf32_Half	e_phentsize;		/* Program header table entry size */
    Elf32_Half	e_phnum;		/* Program header table entry count */
    Elf32_Half	e_shentsize;		/* Section header table entry size */
    Elf32_Half	e_shnum;		/* Section header table entry count */
    Elf32_Half	e_shstrndx;		/* Section header string table index */
} Elf32_Ehdr;

=cut
    
sub _load_file_header
{
    my $s = shift;

    my $headerlen = 16 + (2 * 2) + (5 * 4) + (6 * 2);
    my $buffer;
    my $nbytes = sysread($s->{_fh}, $buffer, $headerlen);
    if ($nbytes != $headerlen) {
	warn("Failed to read ELF header - read [$nbytes] instead of [$headerlen]");
	return undef;
    }

    my %elf_header;
    @elf_header{qw(magic type machine version entry phoff shoff flags ehsize
		   phentsize phnum shentsize shnum shstrnxdx
		   )} = unpack('a16s2i5s6', $buffer);

    unless ($elf_header{magic} =~ "^\x7fELF") {
	warn("Bad elf file header magic for: ", $s->file)
	    unless $s->{_suppress_warn_on_bad_magic};
	return undef;
    }

    $s->{_elf_header} = \%elf_header;
    return scalar keys %elf_header;
}



sub _load_sections
{
    my $s = shift;

    my $SHN_UNDEF = 0;
    my @byte_buffers = $s->_load_table("section header table",
				       $s->{_elf_header}->{shoff},
				       $s->{_elf_header}->{shnum},
				       $s->{_elf_header}->{shentsize});

    unless (@byte_buffers) {
	warn "Failed to load buffers for sections";
	return undef;
    }
    
    my @sections;
    @sections = map { Elf::Section->new($_) } @byte_buffers;
    $s->{_sections} = \@sections;

    return scalar @sections;
}

sub _load_segments
{
    my $s = shift;

    my $SHN_UNDEF = 0;
    my @byte_buffers = $s->_load_table("program header table",
				       $s->{_elf_header}->{phoff},
				       $s->{_elf_header}->{phnum},
				       $s->{_elf_header}->{phentsize});

    unless (@byte_buffers) {
	warn "Failed to load buffers for program header segments";
	return undef;
    }
    
    my @segments;
    @segments = map { Elf::Segment->new($_) } @byte_buffers;
    $s->{_segments} = \@segments;

    return scalar @segments;
}

sub _load_table
{
    my $s = shift;
    my $table_name = shift;
    my $offset = shift;
    my $num_chunks = shift;
    my $chunk_size = shift;

    if ($offset == 0) {
	warn("No $table_name");
	return ();
    }

    unless ($num_chunks > 0) {
	warn("Invalid number of chunks in $table_name");
	return ();
    }
    
    my @buffers;
    seek($s->{_fh}, $offset, 0);
    my $big_buffer;
    my $read_size = $chunk_size * $num_chunks;
    my $nbytes = sysread($s->{_fh}, $big_buffer, $read_size);
    
    if ($nbytes != $read_size) {
	warn("Failed to read $read_size at $offset for $table_name");
	return ();
    }
    
    while (length $big_buffer > 0) {
	my $buffer = substr($big_buffer, 0, $chunk_size, "");
	push @buffers, $buffer;
    }
    
    return @buffers;
}

sub num_sections
{
    my $s = shift;
    return scalar $s->sections;
}

sub num_segments
{
    my $s = shift;
    return scalar $s->segments;
}

sub segments
{
    my $s = shift;

    return @{$s->{_segments}};
}

sub loadable_segments
{
    my $s = shift;
    return grep { $_->is_load } $s->segments;
}

sub section
{
    my $s = shift;
    my $index = shift;
    my @sections = $s->sections;
    return $sections[$index];
}

sub section_by_name
{
    my $s = shift;
    my $name = shift;
    foreach my $section ($s->sections) {
	return $section if $section->name eq $name;
    }
    return undef;
}

sub mappable_sections
{
    my $s = shift;
    return grep { $_->addr != 0 } $s->sections;
}

sub sections
{
    my $s = shift;

    $s->_lazy_load_sections;

    return @{$s->{_sections}};
}

=pod

/* Legal values for e_type (object file type).  */

#define ET_NONE		0		/* No file type */
#define ET_REL		1		/* Relocatable file */
#define ET_EXEC		2		/* Executable file */
#define ET_DYN		3		/* Shared object file */
#define ET_CORE		4		/* Core file */
#define	ET_NUM		5		/* Number of defined types */
#define ET_LOOS		0xfe00		/* OS-specific range start */
#define ET_HIOS		0xfeff		/* OS-specific range end */
#define ET_LOPROC	0xff00		/* Processor-specific range start */
#define ET_HIPROC	0xffff		/* Processor-specific range end */

=cut

sub e_type
{
    my $s = shift;
    return undef unless $s->{_elf_header};
    my $type = $s->{_elf_header}->{type};
    my $ET_NUM = 5;
    return undef unless defined $type && $type < $ET_NUM;
    my @types = qw/NONE REL EXEC DYN CORE/;
	
    return $types[$type];
}

sub is_executable
{
    my $s = shift;
    return $s->e_type eq 'EXEC';
}

# ------------------------------------------------------------
# Wrap one ELF section
package Elf::Section;

sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = {};
    $s->{_buffer} = shift;
    bless $s, $c;
    return undef unless $s->_unpack_buffer;
    return $s;
}

sub set_string_section
{
    my $s = shift;
    $s->{_string_section} = shift;
}

sub set_fh
{
    my $s = shift;
    $s->{_fh} = shift;
}


=pod

A single section header.
    
typedef struct
{
    Elf32_Word	sh_name;		/* Section name (string tbl index) */
    Elf32_Word	sh_type;		/* Section type */
    Elf32_Word	sh_flags;		/* Section flags */
    Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
    Elf32_Off	sh_offset;		/* Section file offset */
    Elf32_Word	sh_size;		/* Section size in bytes */
    Elf32_Word	sh_link;		/* Link to another section */
    Elf32_Word	sh_info;		/* Additional section information */
    Elf32_Word	sh_addralign;		/* Section alignment */
    Elf32_Word	sh_entsize;		/* Entry size if section holds table */
} Elf32_Shdr;

=cut

sub _unpack_buffer
{
    my $s = shift;
    
    my @fieldnames = map {"_$_"}
    qw(name type flags addr offset size link info addralign entsize);
    my %fields;
    @{$s}{@fieldnames} = unpack('i10', $s->{_buffer});
    $s->{_file_range} = Range->new($s->offset, $s->offset + $s->size);
    $s->{_mem_range} = Range->new($s->addr, $s->addr + $s->size);
    return 1;
}

sub offset { return $_[0]->{_offset}; }
sub addr { return $_[0]->{_addr}; }
sub size { return $_[0]->{_size}; }
sub link { return $_[0]->{_link}; }
sub file_range { return $_[0]->{_file_range}; }
sub mem_range { return $_[0]->{_mem_range}; }

sub name
{
    my $s = shift;
    return $s->{_string_section}->find_string($s->{_name});
}

=pod

/* Legal values for sh_type (section type).  */

#define SHT_NULL	  0		/* Section header table entry unused */
#define SHT_PROGBITS	  1		/* Program data */
#define SHT_SYMTAB	  2		/* Symbol table */
#define SHT_STRTAB	  3		/* String table */
#define SHT_RELA	  4		/* Relocation entries with addends */
#define SHT_HASH	  5		/* Symbol hash table */
#define SHT_DYNAMIC	  6		/* Dynamic linking information */
#define SHT_NOTE	  7		/* Notes */
#define SHT_NOBITS	  8		/* Program space with no data (bss) */
#define SHT_REL		  9		/* Relocation entries, no addends */
#define SHT_SHLIB	  10		/* Reserved */
#define SHT_DYNSYM	  11		/* Dynamic linker symbol table */
#define SHT_INIT_ARRAY	  14		/* Array of constructors */
#define SHT_FINI_ARRAY	  15		/* Array of destructors */
#define SHT_PREINIT_ARRAY 16		/* Array of pre-constructors */
#define SHT_GROUP	  17		/* Section group */
#define SHT_SYMTAB_SHNDX  18		/* Extended section indeces */
#define	SHT_NUM		  19		/* Number of defined types.  */

=cut

sub sh_type
{
    my $s = shift;
    my $type = $s->{_type};
    my $SHT_NUM = 19;
    return undef unless defined $type && $type < $SHT_NUM;
    my @types = qw/NULL PROGBITS SYMTAB STRTAB RELA HASH DYNAMIC NOTE NOBITS
	REL SHLIB DYNSYM INIT_ARRAY FINI_ARRAY PREINIT_ARRAY GROUP SYMTAX_SHNDX/;
    return $types[$type];
}

sub is_null
{
    my $s = shift;
    return (!defined $s->sh_type) || ($s->sh_type eq 'NULL');
}

sub is_string_table
{
    my $s = shift;
    return !$s->is_null && $s->sh_type eq 'STRTAB';
}

sub is_symbol_table
{
    my $s = shift;
    return !$s->is_null && $s->sh_type eq 'SYMTAB';
}

sub is_nobits
{
    my $s = shift;
    return !$s->is_null && $s->sh_type eq 'NOBITS';
}

sub find_string
{
    my $s = shift;
    return undef unless $s->is_string_table;
    my $index = shift;
    return "" unless defined $index;

#    print "looking up index $index\n";
    
    # Read zero terminated string
    my $offset = $s->{_offset} + $index;
    my $MAX_STRING_SIZE = 1024;
    my $name;
    seek($s->{_fh}, $offset, 0);
#    print "reading at offset: ", sprintf("%08X", $offset), "\n";
    sysread($s->{_fh}, $name, $MAX_STRING_SIZE);
    $name =~ s/\x00.*$//s;
    return $name;
}

sub symbols
{
    my $s = shift;
    $s->load_symbols unless $s->{_sorted_symbol_table};
    return @{$s->{_sorted_symbol_table}};
}

sub find_symbols_in_mem_range
{
    my $s = shift;
    my $r = shift;

    my @symbols;
    foreach my $symbol ($s->symbols) {
	push @symbols, $symbol
	    if $symbol->is_defined && $r->overlaps($symbol->range);
    }

    return @symbols;
}

sub load_symbols
{
    my $s = shift;
    my $string_table = shift;

    return undef unless $s->is_symbol_table;

    my $symentry_size = $s->{_entsize};
    unless ($symentry_size > 0) {
	warn("Invalid symbol table entry size");
	return undef;
    }
    my $total_size = $s->size;
    my $read_size;
    my $buffer;

    my @symbols;
    seek($s->{_fh}, $s->{_offset}, 0);
    for ($read_size = 0;
	 $read_size < $total_size;
	 $read_size += $symentry_size) {
	my $nbytes = sysread($s->{_fh}, $buffer, $symentry_size);
	if ($nbytes != $symentry_size) {
	    warn("Failed to read symbol table entry");
	    return undef;
	}

	my $symbol = Elf::Symbol->new($buffer);
	unless ($symbol) {
	    warn("Error unpacking symbol");
	    return undef;
	}
	$symbol->set_string_table($string_table);

	push @symbols, $symbol;
    }

    @symbols = sort { $a->value <=> $b->value } @symbols;
    
    $s->{_sorted_symbol_table} = \@symbols;
    
    return 1;
}


package Elf::Symbol;

sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = {};
    $s->{_buffer} = shift;
    bless $s, $c;
    return undef unless $s->_unpack_buffer;
    return $s;
}


=pod
    
typedef struct
{
  Elf32_Word	st_name;		/* Symbol name (string tbl index) */
  Elf32_Addr	st_value;		/* Symbol value */
  Elf32_Word	st_size;		/* Symbol size */
  unsigned char	st_info;		/* Symbol type and binding */
  unsigned char	st_other;		/* Symbol visibility */
  Elf32_Section	st_shndx;		/* Section index */
} Elf32_Sym;

=cut


sub _unpack_buffer
{
    my $s = shift;

    my @fieldnames = map {"_$_"} qw(name value size info other shndx);
    my %fields;
    @{$s}{@fieldnames} = unpack('i3C2s1', $s->{_buffer});

    # bind and type are packed into info as low and high nibbles
    $s->{_bind} = ($s->{_info} & 0xf0) >> 4;
    $s->{_type} = ($s->{_info} & 0xf);
    $s->{_range} = Range->new($s->value, $s->value + $s->size);
    return 1;
}

sub range { return $_[0]->{_range}; }
sub value { return $_[0]->{_value}; }
sub size { return $_[0]->{_size}; }

=pod
    
/* Legal values for ST_TYPE subfield of st_info (symbol type).  */

#define STT_NOTYPE	0		/* Symbol type is unspecified */
#define STT_OBJECT	1		/* Symbol is a data object */
#define STT_FUNC	2		/* Symbol is a code object */
#define STT_SECTION	3		/* Symbol associated with a section */
#define STT_FILE	4		/* Symbol's name is file name */
#define STT_COMMON	5		/* Symbol is a common data object */
#define STT_TLS		6		/* Symbol is thread-local data object*/
#define	STT_NUM		7		/* Number of defined types.  */
#define STT_LOOS	10		/* Start of OS-specific */
#define STT_HIOS	12		/* End of OS-specific */
#define STT_LOPROC	13		/* Start of processor-specific */
#define STT_HIPROC	15		/* End of processor-specific */

=cut
    
sub st_type
{
    my $s = shift;
    my $type = $s->{_type};
    return undef unless defined $type && $type <= 15;
    return "OS-SPECIFIC" if $type >= 10 && $type <= 12;
    return "PROC-SPECIFIC" if $type >= 13 && $type <= 15;
    return undef unless $type < 7;
    my @types = qw/NOTYPE OBJECT FUNC SECTION FILE COMMON TLS/;
	
    return $types[$type];
}

sub is_func { return $_[0]->st_type eq 'FUNC' }
sub is_data { return $_[0]->st_type eq 'OBJECT' }
sub is_file { return $_[0]->st_type eq 'FILE' }
sub is_section { return $_[0]->st_type eq 'SECTION' }

sub set_string_table
{
    my $s = shift;
    $s->{_string_table} = shift;
    return 1;
}
    

sub name
{
    my $s = shift;
    my $string_table = $s->{_string_table};
    return undef unless $string_table;
    return $string_table->find_string($s->{_name});
}


sub is_defined
{
    my $s = shift;
    return $s->value && $s->name;
}


# ------------------------------------------------------------
# Wrap one ELF program header segment
package Elf::Segment;

sub new
{
    my $c = shift;
    $c = ref $c if ref $c;
    my $s = {};
    $s->{_buffer} = shift;
    bless $s, $c;
    return undef unless $s->_unpack_buffer;
    return $s;
}

=pod

/* Program segment header.  */

typedef struct
{
  Elf32_Word	p_type;			/* Segment type */
  Elf32_Off	p_offset;		/* Segment file offset */
  Elf32_Addr	p_vaddr;		/* Segment virtual address */
  Elf32_Addr	p_paddr;		/* Segment physical address */
  Elf32_Word	p_filesz;		/* Segment size in file */
  Elf32_Word	p_memsz;		/* Segment size in memory */
  Elf32_Word	p_flags;		/* Segment flags */
  Elf32_Word	p_align;		/* Segment alignment */
} Elf32_Phdr;

=cut
    
sub _unpack_buffer
{
    my $s = shift;
    
    my @fieldnames = map {"_$_"}
    qw(type offset vaddr paddr filesz memsz flags align);
    my %fields;
    @{$s}{@fieldnames} = unpack('i8', $s->{_buffer});
    $s->{_file_range} = Range->new($s->offset, $s->offset + $s->file_size);
    $s->{_mem_range} = Range->new($s->vaddr, $s->vaddr + $s->mem_size);
    return 1;
}

=pod

/* Legal values for p_type (segment type).  */

#define	PT_NULL		0		/* Program header table entry unused */
#define PT_LOAD		1		/* Loadable program segment */
#define PT_DYNAMIC	2		/* Dynamic linking information */
#define PT_INTERP	3		/* Program interpreter */
#define PT_NOTE		4		/* Auxiliary information */
#define PT_SHLIB	5		/* Reserved */
#define PT_PHDR		6		/* Entry for header table itself */
#define PT_TLS		7		/* Thread-local storage segment */
#define	PT_NUM		8		/* Number of defined types */
#define PT_LOOS		0x60000000	/* Start of OS-specific */
#define PT_GNU_EH_FRAME	0x6474e550	/* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK	0x6474e551	/* Indicates stack executability */
#define PT_GNU_RELRO	0x6474e552	/* Read-only after relocation */
#define PT_LOSUNW	0x6ffffffa
#define PT_SUNWBSS	0x6ffffffa	/* Sun Specific segment */
#define PT_SUNWSTACK	0x6ffffffb	/* Stack segment */
#define PT_HISUNW	0x6fffffff
#define PT_HIOS		0x6fffffff	/* End of OS-specific */
#define PT_LOPROC	0x70000000	/* Start of processor-specific */
#define PT_HIPROC	0x7fffffff	/* End of processor-specific */

=cut

use constant PT_LOAD => 1;    
sub is_load
{
    my $s = shift;
    return $s->type == PT_LOAD;
}

=pod
    
/* Legal values for p_flags (segment flags).  */

#define PF_X		(1 << 0)	/* Segment is executable */
#define PF_W		(1 << 1)	/* Segment is writable */
#define PF_R		(1 << 2)	/* Segment is readable */
#define PF_MASKOS	0x0ff00000	/* OS-specific */
#define PF_MASKPROC	0xf0000000	/* Processor-specific */

=cut

use constant PF_X => 1;
use constant PF_W => 2;
use constant PF_R => 4;    
    
sub is_readable
{
    my $s = shift;
    return $s->_flag_is_set(PF_R);
}
sub is_writable
{
    my $s = shift;
    return $s->_flag_is_set(PF_W);
}
sub is_executable
{
    my $s = shift;
    return $s->_flag_is_set(PF_X);
}

sub _flag_is_set
{
    my $s = shift;
    my $bitflag = shift;
    my $flags = $s->{_flags};
    return $bitflag & $flags;
}

sub offset { return $_[0]->{_offset}; }
sub vaddr { return $_[0]->{_vaddr}; }
sub file_size { return $_[0]->{_filesz}; }
sub mem_size { return $_[0]->{_memsz}; }
sub file_range { return $_[0]->{_file_range}; }
sub mem_range { return $_[0]->{_mem_range}; }
sub type { return $_[0]->{_type}; }


1;
