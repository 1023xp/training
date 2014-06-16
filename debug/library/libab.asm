
libab.so:     file format elf32-i386


Disassembly of section .init:

00000310 <_init>:
 310:	53                   	push   %ebx
 311:	83 ec 08             	sub    $0x8,%esp
 314:	e8 00 00 00 00       	call   319 <_init+0x9>
 319:	5b                   	pop    %ebx
 31a:	81 c3 db 1c 00 00    	add    $0x1cdb,%ebx
 320:	8b 83 f8 ff ff ff    	mov    -0x8(%ebx),%eax
 326:	85 c0                	test   %eax,%eax
 328:	74 05                	je     32f <_init+0x1f>
 32a:	e8 31 00 00 00       	call   360 <__gmon_start__@plt>
 32f:	e8 cc 00 00 00       	call   400 <frame_dummy>
 334:	e8 27 01 00 00       	call   460 <__do_global_ctors_aux>
 339:	83 c4 08             	add    $0x8,%esp
 33c:	5b                   	pop    %ebx
 33d:	c3                   	ret    

Disassembly of section .plt:

00000340 <__cxa_finalize@plt-0x10>:
 340:	ff b3 04 00 00 00    	pushl  0x4(%ebx)
 346:	ff a3 08 00 00 00    	jmp    *0x8(%ebx)
 34c:	00 00                	add    %al,(%eax)
	...

00000350 <__cxa_finalize@plt>:
 350:	ff a3 0c 00 00 00    	jmp    *0xc(%ebx)
 356:	68 00 00 00 00       	push   $0x0
 35b:	e9 e0 ff ff ff       	jmp    340 <_init+0x30>

00000360 <__gmon_start__@plt>:
 360:	ff a3 10 00 00 00    	jmp    *0x10(%ebx)
 366:	68 08 00 00 00       	push   $0x8
 36b:	e9 d0 ff ff ff       	jmp    340 <_init+0x30>

00000370 <a@plt>:
 370:	ff a3 14 00 00 00    	jmp    *0x14(%ebx)
 376:	68 10 00 00 00       	push   $0x10
 37b:	e9 c0 ff ff ff       	jmp    340 <_init+0x30>

Disassembly of section .text:

00000380 <__do_global_dtors_aux>:
 380:	55                   	push   %ebp
 381:	89 e5                	mov    %esp,%ebp
 383:	56                   	push   %esi
 384:	53                   	push   %ebx
 385:	e8 ad 00 00 00       	call   437 <__i686.get_pc_thunk.bx>
 38a:	81 c3 6a 1c 00 00    	add    $0x1c6a,%ebx
 390:	83 ec 10             	sub    $0x10,%esp
 393:	80 bb 1c 00 00 00 00 	cmpb   $0x0,0x1c(%ebx)
 39a:	75 5d                	jne    3f9 <__do_global_dtors_aux+0x79>
 39c:	8b 83 f4 ff ff ff    	mov    -0xc(%ebx),%eax
 3a2:	85 c0                	test   %eax,%eax
 3a4:	74 0e                	je     3b4 <__do_global_dtors_aux+0x34>
 3a6:	8b 83 18 00 00 00    	mov    0x18(%ebx),%eax
 3ac:	89 04 24             	mov    %eax,(%esp)
 3af:	e8 9c ff ff ff       	call   350 <__cxa_finalize@plt>
 3b4:	8b 83 20 00 00 00    	mov    0x20(%ebx),%eax
 3ba:	8d b3 24 ff ff ff    	lea    -0xdc(%ebx),%esi
 3c0:	8d 93 20 ff ff ff    	lea    -0xe0(%ebx),%edx
 3c6:	29 d6                	sub    %edx,%esi
 3c8:	c1 fe 02             	sar    $0x2,%esi
 3cb:	83 ee 01             	sub    $0x1,%esi
 3ce:	39 f0                	cmp    %esi,%eax
 3d0:	73 20                	jae    3f2 <__do_global_dtors_aux+0x72>
 3d2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 3d8:	83 c0 01             	add    $0x1,%eax
 3db:	89 83 20 00 00 00    	mov    %eax,0x20(%ebx)
 3e1:	ff 94 83 20 ff ff ff 	call   *-0xe0(%ebx,%eax,4)
 3e8:	8b 83 20 00 00 00    	mov    0x20(%ebx),%eax
 3ee:	39 f0                	cmp    %esi,%eax
 3f0:	72 e6                	jb     3d8 <__do_global_dtors_aux+0x58>
 3f2:	c6 83 1c 00 00 00 01 	movb   $0x1,0x1c(%ebx)
 3f9:	83 c4 10             	add    $0x10,%esp
 3fc:	5b                   	pop    %ebx
 3fd:	5e                   	pop    %esi
 3fe:	5d                   	pop    %ebp
 3ff:	c3                   	ret    

00000400 <frame_dummy>:
 400:	55                   	push   %ebp
 401:	89 e5                	mov    %esp,%ebp
 403:	53                   	push   %ebx
 404:	e8 2e 00 00 00       	call   437 <__i686.get_pc_thunk.bx>
 409:	81 c3 eb 1b 00 00    	add    $0x1beb,%ebx
 40f:	83 ec 14             	sub    $0x14,%esp
 412:	8b 93 28 ff ff ff    	mov    -0xd8(%ebx),%edx
 418:	85 d2                	test   %edx,%edx
 41a:	74 15                	je     431 <frame_dummy+0x31>
 41c:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 422:	85 c0                	test   %eax,%eax
 424:	74 0b                	je     431 <frame_dummy+0x31>
 426:	8d 93 28 ff ff ff    	lea    -0xd8(%ebx),%edx
 42c:	89 14 24             	mov    %edx,(%esp)
 42f:	ff d0                	call   *%eax
 431:	83 c4 14             	add    $0x14,%esp
 434:	5b                   	pop    %ebx
 435:	5d                   	pop    %ebp
 436:	c3                   	ret    

00000437 <__i686.get_pc_thunk.bx>:
 437:	8b 1c 24             	mov    (%esp),%ebx
 43a:	c3                   	ret    
 43b:	90                   	nop

0000043c <a>:
 43c:	55                   	push   %ebp
 43d:	89 e5                	mov    %esp,%ebp
 43f:	5d                   	pop    %ebp
 440:	c3                   	ret    

00000441 <b>:
 441:	55                   	push   %ebp
 442:	89 e5                	mov    %esp,%ebp
 444:	53                   	push   %ebx
 445:	83 ec 04             	sub    $0x4,%esp
 448:	e8 ea ff ff ff       	call   437 <__i686.get_pc_thunk.bx>
 44d:	81 c3 a7 1b 00 00    	add    $0x1ba7,%ebx
 453:	e8 18 ff ff ff       	call   370 <a@plt>
 458:	eb f9                	jmp    453 <b+0x12>
 45a:	90                   	nop
 45b:	90                   	nop
 45c:	90                   	nop
 45d:	90                   	nop
 45e:	90                   	nop
 45f:	90                   	nop

00000460 <__do_global_ctors_aux>:
 460:	55                   	push   %ebp
 461:	89 e5                	mov    %esp,%ebp
 463:	56                   	push   %esi
 464:	53                   	push   %ebx
 465:	e8 cd ff ff ff       	call   437 <__i686.get_pc_thunk.bx>
 46a:	81 c3 8a 1b 00 00    	add    $0x1b8a,%ebx
 470:	8b 83 18 ff ff ff    	mov    -0xe8(%ebx),%eax
 476:	83 f8 ff             	cmp    $0xffffffff,%eax
 479:	74 19                	je     494 <__do_global_ctors_aux+0x34>
 47b:	8d b3 18 ff ff ff    	lea    -0xe8(%ebx),%esi
 481:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 488:	83 ee 04             	sub    $0x4,%esi
 48b:	ff d0                	call   *%eax
 48d:	8b 06                	mov    (%esi),%eax
 48f:	83 f8 ff             	cmp    $0xffffffff,%eax
 492:	75 f4                	jne    488 <__do_global_ctors_aux+0x28>
 494:	5b                   	pop    %ebx
 495:	5e                   	pop    %esi
 496:	5d                   	pop    %ebp
 497:	c3                   	ret    

Disassembly of section .fini:

00000498 <_fini>:
 498:	53                   	push   %ebx
 499:	83 ec 08             	sub    $0x8,%esp
 49c:	e8 00 00 00 00       	call   4a1 <_fini+0x9>
 4a1:	5b                   	pop    %ebx
 4a2:	81 c3 53 1b 00 00    	add    $0x1b53,%ebx
 4a8:	e8 d3 fe ff ff       	call   380 <__do_global_dtors_aux>
 4ad:	83 c4 08             	add    $0x8,%esp
 4b0:	5b                   	pop    %ebx
 4b1:	c3                   	ret    
