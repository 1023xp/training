# apps.mk

#add other applications after appexam
apps: appexam

OUTAPPEXAM = $(OUTAPP)/appexam
APPEXAMDIR = $(APPDIR)/example
APPINSTDIR = $(IMGRFS)/usr/local/bin

appexam:
	@$(ECHO) -n "Making SYJC appexam"
	@$(MKDIR) $(OUTAPPEXAM)
	@$(LNDIR) $(APPEXAMDIR) $(OUTAPPEXAM)
	@$(MAKE) -C $(OUTAPPEXAM)
	@$(MKDIR) -p $(APPINSTDIR)
	@$(CP) $(OUTAPPEXAM)/syjc-app-exam $(APPINSTDIR)
	@$(ECHO) "Done"

#add more applications here
