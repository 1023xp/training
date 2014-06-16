
test1:     file format elf32-i386

Disassembly of section .init:

08048338 <_init>:
 8048338:	55                   	push   %ebp
 8048339:	89 e5                	mov    %esp,%ebp
 804833b:	53                   	push   %ebx
 804833c:	83 ec 04             	sub    $0x4,%esp
 804833f:	e8 00 00 00 00       	call   8048344 <_init+0xc>
 8048344:	5b                   	pop    %ebx
 8048345:	81 c3 0c 14 00 00    	add    $0x140c,%ebx
 804834b:	8b 93 f8 ff ff ff    	mov    -0x8(%ebx),%edx
 8048351:	85 d2                	test   %edx,%edx
 8048353:	74 05                	je     804835a <_init+0x22>
 8048355:	e8 b6 00 00 00       	call   8048410 <__gmon_start__>
 804835a:	e8 21 01 00 00       	call   8048480 <frame_dummy>
 804835f:	e8 9c 02 00 00       	call   8048600 <__do_global_ctors_aux>
 8048364:	58                   	pop    %eax
 8048365:	5b                   	pop    %ebx
 8048366:	c9                   	leave  
 8048367:	c3                   	ret    
Disassembly of section .plt:

08048368 <_mcleanup@plt-0x10>:
 8048368:	ff 35 54 97 04 08    	pushl  0x8049754
 804836e:	ff 25 58 97 04 08    	jmp    *0x8049758
 8048374:	00 00                	add    %al,(%eax)
	...

08048378 <_mcleanup@plt>:
 8048378:	ff 25 5c 97 04 08    	jmp    *0x804975c
 804837e:	68 00 00 00 00       	push   $0x0
 8048383:	e9 e0 ff ff ff       	jmp    8048368 <_init+0x30>

08048388 <__cxa_atexit@plt>:
 8048388:	ff 25 60 97 04 08    	jmp    *0x8049760
 804838e:	68 08 00 00 00       	push   $0x8
 8048393:	e9 d0 ff ff ff       	jmp    8048368 <_init+0x30>

08048398 <__libc_start_main@plt>:
 8048398:	ff 25 64 97 04 08    	jmp    *0x8049764
 804839e:	68 10 00 00 00       	push   $0x10
 80483a3:	e9 c0 ff ff ff       	jmp    8048368 <_init+0x30>

080483a8 <__monstartup@plt>:
 80483a8:	ff 25 68 97 04 08    	jmp    *0x8049768
 80483ae:	68 18 00 00 00       	push   $0x18
 80483b3:	e9 b0 ff ff ff       	jmp    8048368 <_init+0x30>

080483b8 <mcount@plt>:
 80483b8:	ff 25 6c 97 04 08    	jmp    *0x804976c
 80483be:	68 20 00 00 00       	push   $0x20
 80483c3:	e9 a0 ff ff ff       	jmp    8048368 <_init+0x30>

080483c8 <puts@plt>:
 80483c8:	ff 25 70 97 04 08    	jmp    *0x8049770
 80483ce:	68 28 00 00 00       	push   $0x28
 80483d3:	e9 90 ff ff ff       	jmp    8048368 <_init+0x30>
Disassembly of section .text:

080483e0 <_start>:
 80483e0:	31 ed                	xor    %ebp,%ebp
 80483e2:	5e                   	pop    %esi
 80483e3:	89 e1                	mov    %esp,%ecx
 80483e5:	83 e4 f0             	and    $0xfffffff0,%esp
 80483e8:	50                   	push   %eax
 80483e9:	54                   	push   %esp
 80483ea:	52                   	push   %edx
 80483eb:	68 50 85 04 08       	push   $0x8048550
 80483f0:	68 60 85 04 08       	push   $0x8048560
 80483f5:	51                   	push   %ecx
 80483f6:	56                   	push   %esi
 80483f7:	68 ee 84 04 08       	push   $0x80484ee
 80483fc:	e8 97 ff ff ff       	call   8048398 <__libc_start_main@plt>
 8048401:	f4                   	hlt    
 8048402:	90                   	nop    
 8048403:	90                   	nop    
 8048404:	90                   	nop    
 8048405:	90                   	nop    
 8048406:	90                   	nop    
 8048407:	90                   	nop    
 8048408:	90                   	nop    
 8048409:	90                   	nop    
 804840a:	90                   	nop    
 804840b:	90                   	nop    
 804840c:	90                   	nop    
 804840d:	90                   	nop    
 804840e:	90                   	nop    
 804840f:	90                   	nop    

08048410 <__gmon_start__>:
 8048410:	55                   	push   %ebp
 8048411:	89 e5                	mov    %esp,%ebp
 8048413:	83 ec 08             	sub    $0x8,%esp
 8048416:	a1 80 97 04 08       	mov    0x8049780,%eax
 804841b:	85 c0                	test   %eax,%eax
 804841d:	74 02                	je     8048421 <__gmon_start__+0x11>
 804841f:	c9                   	leave  
 8048420:	c3                   	ret    
 8048421:	c7 05 80 97 04 08 01 	movl   $0x1,0x8049780
 8048428:	00 00 00 
 804842b:	c7 44 24 04 44 86 04 	movl   $0x8048644,0x4(%esp)
 8048432:	08 
 8048433:	c7 04 24 e0 83 04 08 	movl   $0x80483e0,(%esp)
 804843a:	e8 69 ff ff ff       	call   80483a8 <__monstartup@plt>
 804843f:	c7 04 24 78 83 04 08 	movl   $0x8048378,(%esp)
 8048446:	e8 75 01 00 00       	call   80485c0 <atexit>
 804844b:	c9                   	leave  
 804844c:	c3                   	ret    
 804844d:	90                   	nop    
 804844e:	90                   	nop    
 804844f:	90                   	nop    

08048450 <__do_global_dtors_aux>:
 8048450:	55                   	push   %ebp
 8048451:	89 e5                	mov    %esp,%ebp
 8048453:	83 ec 08             	sub    $0x8,%esp
 8048456:	80 3d 84 97 04 08 00 	cmpb   $0x0,0x8049784
 804845d:	74 0c                	je     804846b <__do_global_dtors_aux+0x1b>
 804845f:	eb 1c                	jmp    804847d <__do_global_dtors_aux+0x2d>
 8048461:	83 c0 04             	add    $0x4,%eax
 8048464:	a3 7c 97 04 08       	mov    %eax,0x804977c
 8048469:	ff d2                	call   *%edx
 804846b:	a1 7c 97 04 08       	mov    0x804977c,%eax
 8048470:	8b 10                	mov    (%eax),%edx
 8048472:	85 d2                	test   %edx,%edx
 8048474:	75 eb                	jne    8048461 <__do_global_dtors_aux+0x11>
 8048476:	c6 05 84 97 04 08 01 	movb   $0x1,0x8049784
 804847d:	c9                   	leave  
 804847e:	c3                   	ret    
 804847f:	90                   	nop    

08048480 <frame_dummy>:
 8048480:	55                   	push   %ebp
 8048481:	89 e5                	mov    %esp,%ebp
 8048483:	83 ec 08             	sub    $0x8,%esp
 8048486:	a1 74 96 04 08       	mov    0x8049674,%eax
 804848b:	85 c0                	test   %eax,%eax
 804848d:	74 12                	je     80484a1 <frame_dummy+0x21>
 804848f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048494:	85 c0                	test   %eax,%eax
 8048496:	74 09                	je     80484a1 <frame_dummy+0x21>
 8048498:	c7 04 24 74 96 04 08 	movl   $0x8049674,(%esp)
 804849f:	ff d0                	call   *%eax
 80484a1:	c9                   	leave  
 80484a2:	c3                   	ret    
 80484a3:	90                   	nop    

080484a4 <a>:
 80484a4:	55                   	push   %ebp
 80484a5:	89 e5                	mov    %esp,%ebp
 80484a7:	83 ec 10             	sub    $0x10,%esp
 80484aa:	e8 09 ff ff ff       	call   80483b8 <mcount@plt>
 80484af:	c7 45 fc 00 2d 31 01 	movl   $0x1312d00,-0x4(%ebp)
 80484b6:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484b9:	83 e8 01             	sub    $0x1,%eax
 80484bc:	89 45 fc             	mov    %eax,-0x4(%ebp)
 80484bf:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484c2:	83 f8 ff             	cmp    $0xffffffff,%eax
 80484c5:	75 ef                	jne    80484b6 <a+0x12>
 80484c7:	c9                   	leave  
 80484c8:	c3                   	ret    

080484c9 <c>:
 80484c9:	55                   	push   %ebp
 80484ca:	89 e5                	mov    %esp,%ebp
 80484cc:	83 ec 10             	sub    $0x10,%esp
 80484cf:	e8 e4 fe ff ff       	call   80483b8 <mcount@plt>
 80484d4:	c7 45 fc 00 5a 62 02 	movl   $0x2625a00,-0x4(%ebp)
 80484db:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484de:	83 e8 01             	sub    $0x1,%eax
 80484e1:	89 45 fc             	mov    %eax,-0x4(%ebp)
 80484e4:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80484e7:	83 f8 ff             	cmp    $0xffffffff,%eax
 80484ea:	75 ef                	jne    80484db <c+0x12>
 80484ec:	c9                   	leave  
 80484ed:	c3                   	ret    

080484ee <main>:
 80484ee:	8d 4c 24 04          	lea    0x4(%esp),%ecx
 80484f2:	83 e4 f0             	and    $0xfffffff0,%esp
 80484f5:	ff 71 fc             	pushl  -0x4(%ecx)
 80484f8:	55                   	push   %ebp
 80484f9:	89 e5                	mov    %esp,%ebp
 80484fb:	51                   	push   %ecx
 80484fc:	83 ec 04             	sub    $0x4,%esp
 80484ff:	e8 b4 fe ff ff       	call   80483b8 <mcount@plt>
 8048504:	c7 04 24 4c 86 04 08 	movl   $0x804864c,(%esp)
 804850b:	e8 b8 fe ff ff       	call   80483c8 <puts@plt>
 8048510:	e8 0b 00 00 00       	call   8048520 <b>
 8048515:	83 c4 04             	add    $0x4,%esp
 8048518:	59                   	pop    %ecx
 8048519:	5d                   	pop    %ebp
 804851a:	8d 61 fc             	lea    -0x4(%ecx),%esp
 804851d:	c3                   	ret    
 804851e:	90                   	nop    
 804851f:	90                   	nop    

08048520 <b>:
 8048520:	55                   	push   %ebp
 8048521:	89 e5                	mov    %esp,%ebp
 8048523:	83 ec 18             	sub    $0x18,%esp
 8048526:	e8 8d fe ff ff       	call   80483b8 <mcount@plt>
 804852b:	c7 45 fc 40 4b 4c 00 	movl   $0x4c4b40,-0x4(%ebp)
 8048532:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048535:	83 e8 01             	sub    $0x1,%eax
 8048538:	89 45 fc             	mov    %eax,-0x4(%ebp)
 804853b:	8b 45 fc             	mov    -0x4(%ebp),%eax
 804853e:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048541:	75 ef                	jne    8048532 <b+0x12>
 8048543:	e8 5c ff ff ff       	call   80484a4 <a>
 8048548:	e8 7c ff ff ff       	call   80484c9 <c>
 804854d:	c9                   	leave  
 804854e:	c3                   	ret    
 804854f:	90                   	nop    

08048550 <__libc_csu_fini>:
 8048550:	55                   	push   %ebp
 8048551:	89 e5                	mov    %esp,%ebp
 8048553:	5d                   	pop    %ebp
 8048554:	c3                   	ret    
 8048555:	8d 74 26 00          	lea    0x0(%esi),%esi
 8048559:	8d bc 27 00 00 00 00 	lea    0x0(%edi),%edi

08048560 <__libc_csu_init>:
 8048560:	55                   	push   %ebp
 8048561:	89 e5                	mov    %esp,%ebp
 8048563:	57                   	push   %edi
 8048564:	56                   	push   %esi
 8048565:	53                   	push   %ebx
 8048566:	e8 4f 00 00 00       	call   80485ba <__i686.get_pc_thunk.bx>
 804856b:	81 c3 e5 11 00 00    	add    $0x11e5,%ebx
 8048571:	83 ec 0c             	sub    $0xc,%esp
 8048574:	e8 bf fd ff ff       	call   8048338 <_init>
 8048579:	8d bb 14 ff ff ff    	lea    -0xec(%ebx),%edi
 804857f:	8d 83 14 ff ff ff    	lea    -0xec(%ebx),%eax
 8048585:	29 c7                	sub    %eax,%edi
 8048587:	c1 ff 02             	sar    $0x2,%edi
 804858a:	85 ff                	test   %edi,%edi
 804858c:	74 24                	je     80485b2 <__libc_csu_init+0x52>
 804858e:	31 f6                	xor    %esi,%esi
 8048590:	8b 45 10             	mov    0x10(%ebp),%eax
 8048593:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048597:	8b 45 0c             	mov    0xc(%ebp),%eax
 804859a:	89 44 24 04          	mov    %eax,0x4(%esp)
 804859e:	8b 45 08             	mov    0x8(%ebp),%eax
 80485a1:	89 04 24             	mov    %eax,(%esp)
 80485a4:	ff 94 b3 14 ff ff ff 	call   *-0xec(%ebx,%esi,4)
 80485ab:	83 c6 01             	add    $0x1,%esi
 80485ae:	39 f7                	cmp    %esi,%edi
 80485b0:	75 de                	jne    8048590 <__libc_csu_init+0x30>
 80485b2:	83 c4 0c             	add    $0xc,%esp
 80485b5:	5b                   	pop    %ebx
 80485b6:	5e                   	pop    %esi
 80485b7:	5f                   	pop    %edi
 80485b8:	5d                   	pop    %ebp
 80485b9:	c3                   	ret    

080485ba <__i686.get_pc_thunk.bx>:
 80485ba:	8b 1c 24             	mov    (%esp),%ebx
 80485bd:	c3                   	ret    
 80485be:	90                   	nop    
 80485bf:	90                   	nop    

080485c0 <atexit>:
 80485c0:	55                   	push   %ebp
 80485c1:	31 c0                	xor    %eax,%eax
 80485c3:	89 e5                	mov    %esp,%ebp
 80485c5:	53                   	push   %ebx
 80485c6:	e8 ef ff ff ff       	call   80485ba <__i686.get_pc_thunk.bx>
 80485cb:	81 c3 85 11 00 00    	add    $0x1185,%ebx
 80485d1:	83 ec 0c             	sub    $0xc,%esp
 80485d4:	8b 93 fc ff ff ff    	mov    -0x4(%ebx),%edx
 80485da:	85 d2                	test   %edx,%edx
 80485dc:	74 02                	je     80485e0 <atexit+0x20>
 80485de:	8b 02                	mov    (%edx),%eax
 80485e0:	89 44 24 08          	mov    %eax,0x8(%esp)
 80485e4:	8b 45 08             	mov    0x8(%ebp),%eax
 80485e7:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%esp)
 80485ee:	00 
 80485ef:	89 04 24             	mov    %eax,(%esp)
 80485f2:	e8 91 fd ff ff       	call   8048388 <__cxa_atexit@plt>
 80485f7:	83 c4 0c             	add    $0xc,%esp
 80485fa:	5b                   	pop    %ebx
 80485fb:	5d                   	pop    %ebp
 80485fc:	c3                   	ret    
 80485fd:	90                   	nop    
 80485fe:	90                   	nop    
 80485ff:	90                   	nop    

08048600 <__do_global_ctors_aux>:
 8048600:	55                   	push   %ebp
 8048601:	89 e5                	mov    %esp,%ebp
 8048603:	53                   	push   %ebx
 8048604:	bb 64 96 04 08       	mov    $0x8049664,%ebx
 8048609:	83 ec 04             	sub    $0x4,%esp
 804860c:	a1 64 96 04 08       	mov    0x8049664,%eax
 8048611:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048614:	74 0c                	je     8048622 <__do_global_ctors_aux+0x22>
 8048616:	83 eb 04             	sub    $0x4,%ebx
 8048619:	ff d0                	call   *%eax
 804861b:	8b 03                	mov    (%ebx),%eax
 804861d:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048620:	75 f4                	jne    8048616 <__do_global_ctors_aux+0x16>
 8048622:	83 c4 04             	add    $0x4,%esp
 8048625:	5b                   	pop    %ebx
 8048626:	5d                   	pop    %ebp
 8048627:	c3                   	ret    
Disassembly of section .fini:

08048628 <_fini>:
 8048628:	55                   	push   %ebp
 8048629:	89 e5                	mov    %esp,%ebp
 804862b:	53                   	push   %ebx
 804862c:	83 ec 04             	sub    $0x4,%esp
 804862f:	e8 00 00 00 00       	call   8048634 <_fini+0xc>
 8048634:	5b                   	pop    %ebx
 8048635:	81 c3 1c 11 00 00    	add    $0x111c,%ebx
 804863b:	e8 10 fe ff ff       	call   8048450 <__do_global_dtors_aux>
 8048640:	59                   	pop    %ecx
 8048641:	5b                   	pop    %ebx
 8048642:	c9                   	leave  
 8048643:	c3                   	ret    
