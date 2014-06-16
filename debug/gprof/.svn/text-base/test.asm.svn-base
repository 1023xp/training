
test:     file format elf32-i386

Disassembly of section .init:

08048274 <_init>:
 8048274:	55                   	push   %ebp
 8048275:	89 e5                	mov    %esp,%ebp
 8048277:	53                   	push   %ebx
 8048278:	83 ec 04             	sub    $0x4,%esp
 804827b:	e8 00 00 00 00       	call   8048280 <_init+0xc>
 8048280:	5b                   	pop    %ebx
 8048281:	81 c3 4c 13 00 00    	add    $0x134c,%ebx
 8048287:	8b 93 fc ff ff ff    	mov    -0x4(%ebx),%edx
 804828d:	85 d2                	test   %edx,%edx
 804828f:	74 05                	je     8048296 <_init+0x22>
 8048291:	e8 1e 00 00 00       	call   80482b4 <__gmon_start__@plt>
 8048296:	e8 b5 00 00 00       	call   8048350 <frame_dummy>
 804829b:	e8 e0 01 00 00       	call   8048480 <__do_global_ctors_aux>
 80482a0:	58                   	pop    %eax
 80482a1:	5b                   	pop    %ebx
 80482a2:	c9                   	leave  
 80482a3:	c3                   	ret    
Disassembly of section .plt:

080482a4 <__gmon_start__@plt-0x10>:
 80482a4:	ff 35 d0 95 04 08    	pushl  0x80495d0
 80482aa:	ff 25 d4 95 04 08    	jmp    *0x80495d4
 80482b0:	00 00                	add    %al,(%eax)
	...

080482b4 <__gmon_start__@plt>:
 80482b4:	ff 25 d8 95 04 08    	jmp    *0x80495d8
 80482ba:	68 00 00 00 00       	push   $0x0
 80482bf:	e9 e0 ff ff ff       	jmp    80482a4 <_init+0x30>

080482c4 <__libc_start_main@plt>:
 80482c4:	ff 25 dc 95 04 08    	jmp    *0x80495dc
 80482ca:	68 08 00 00 00       	push   $0x8
 80482cf:	e9 d0 ff ff ff       	jmp    80482a4 <_init+0x30>

080482d4 <puts@plt>:
 80482d4:	ff 25 e0 95 04 08    	jmp    *0x80495e0
 80482da:	68 10 00 00 00       	push   $0x10
 80482df:	e9 c0 ff ff ff       	jmp    80482a4 <_init+0x30>
Disassembly of section .text:

080482f0 <_start>:
 80482f0:	31 ed                	xor    %ebp,%ebp
 80482f2:	5e                   	pop    %esi
 80482f3:	89 e1                	mov    %esp,%ecx
 80482f5:	83 e4 f0             	and    $0xfffffff0,%esp
 80482f8:	50                   	push   %eax
 80482f9:	54                   	push   %esp
 80482fa:	52                   	push   %edx
 80482fb:	68 10 84 04 08       	push   $0x8048410
 8048300:	68 20 84 04 08       	push   $0x8048420
 8048305:	51                   	push   %ecx
 8048306:	56                   	push   %esi
 8048307:	68 b4 83 04 08       	push   $0x80483b4
 804830c:	e8 b3 ff ff ff       	call   80482c4 <__libc_start_main@plt>
 8048311:	f4                   	hlt    
 8048312:	90                   	nop    
 8048313:	90                   	nop    
 8048314:	90                   	nop    
 8048315:	90                   	nop    
 8048316:	90                   	nop    
 8048317:	90                   	nop    
 8048318:	90                   	nop    
 8048319:	90                   	nop    
 804831a:	90                   	nop    
 804831b:	90                   	nop    
 804831c:	90                   	nop    
 804831d:	90                   	nop    
 804831e:	90                   	nop    
 804831f:	90                   	nop    

08048320 <__do_global_dtors_aux>:
 8048320:	55                   	push   %ebp
 8048321:	89 e5                	mov    %esp,%ebp
 8048323:	83 ec 08             	sub    $0x8,%esp
 8048326:	80 3d f0 95 04 08 00 	cmpb   $0x0,0x80495f0
 804832d:	74 0c                	je     804833b <__do_global_dtors_aux+0x1b>
 804832f:	eb 1c                	jmp    804834d <__do_global_dtors_aux+0x2d>
 8048331:	83 c0 04             	add    $0x4,%eax
 8048334:	a3 ec 95 04 08       	mov    %eax,0x80495ec
 8048339:	ff d2                	call   *%edx
 804833b:	a1 ec 95 04 08       	mov    0x80495ec,%eax
 8048340:	8b 10                	mov    (%eax),%edx
 8048342:	85 d2                	test   %edx,%edx
 8048344:	75 eb                	jne    8048331 <__do_global_dtors_aux+0x11>
 8048346:	c6 05 f0 95 04 08 01 	movb   $0x1,0x80495f0
 804834d:	c9                   	leave  
 804834e:	c3                   	ret    
 804834f:	90                   	nop    

08048350 <frame_dummy>:
 8048350:	55                   	push   %ebp
 8048351:	89 e5                	mov    %esp,%ebp
 8048353:	83 ec 08             	sub    $0x8,%esp
 8048356:	a1 f4 94 04 08       	mov    0x80494f4,%eax
 804835b:	85 c0                	test   %eax,%eax
 804835d:	74 12                	je     8048371 <frame_dummy+0x21>
 804835f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048364:	85 c0                	test   %eax,%eax
 8048366:	74 09                	je     8048371 <frame_dummy+0x21>
 8048368:	c7 04 24 f4 94 04 08 	movl   $0x80494f4,(%esp)
 804836f:	ff d0                	call   *%eax
 8048371:	c9                   	leave  
 8048372:	c3                   	ret    
 8048373:	90                   	nop    

08048374 <a>:
 8048374:	55                   	push   %ebp
 8048375:	89 e5                	mov    %esp,%ebp
 8048377:	83 ec 10             	sub    $0x10,%esp
 804837a:	c7 45 fc 00 2d 31 01 	movl   $0x1312d00,-0x4(%ebp)
 8048381:	8b 45 fc             	mov    -0x4(%ebp),%eax
 8048384:	83 e8 01             	sub    $0x1,%eax
 8048387:	89 45 fc             	mov    %eax,-0x4(%ebp)
 804838a:	8b 45 fc             	mov    -0x4(%ebp),%eax
 804838d:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048390:	75 ef                	jne    8048381 <a+0xd>
 8048392:	c9                   	leave  
 8048393:	c3                   	ret    

08048394 <c>:
 8048394:	55                   	push   %ebp
 8048395:	89 e5                	mov    %esp,%ebp
 8048397:	83 ec 10             	sub    $0x10,%esp
 804839a:	c7 45 fc 00 5a 62 02 	movl   $0x2625a00,-0x4(%ebp)
 80483a1:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80483a4:	83 e8 01             	sub    $0x1,%eax
 80483a7:	89 45 fc             	mov    %eax,-0x4(%ebp)
 80483aa:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80483ad:	83 f8 ff             	cmp    $0xffffffff,%eax
 80483b0:	75 ef                	jne    80483a1 <c+0xd>
 80483b2:	c9                   	leave  
 80483b3:	c3                   	ret    

080483b4 <main>:
 80483b4:	8d 4c 24 04          	lea    0x4(%esp),%ecx
 80483b8:	83 e4 f0             	and    $0xfffffff0,%esp
 80483bb:	ff 71 fc             	pushl  -0x4(%ecx)
 80483be:	55                   	push   %ebp
 80483bf:	89 e5                	mov    %esp,%ebp
 80483c1:	51                   	push   %ecx
 80483c2:	83 ec 04             	sub    $0x4,%esp
 80483c5:	c7 04 24 cc 84 04 08 	movl   $0x80484cc,(%esp)
 80483cc:	e8 03 ff ff ff       	call   80482d4 <puts@plt>
 80483d1:	e8 0a 00 00 00       	call   80483e0 <b>
 80483d6:	83 c4 04             	add    $0x4,%esp
 80483d9:	59                   	pop    %ecx
 80483da:	5d                   	pop    %ebp
 80483db:	8d 61 fc             	lea    -0x4(%ecx),%esp
 80483de:	c3                   	ret    
 80483df:	90                   	nop    

080483e0 <b>:
 80483e0:	55                   	push   %ebp
 80483e1:	89 e5                	mov    %esp,%ebp
 80483e3:	83 ec 18             	sub    $0x18,%esp
 80483e6:	c7 45 fc 40 4b 4c 00 	movl   $0x4c4b40,-0x4(%ebp)
 80483ed:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80483f0:	83 e8 01             	sub    $0x1,%eax
 80483f3:	89 45 fc             	mov    %eax,-0x4(%ebp)
 80483f6:	8b 45 fc             	mov    -0x4(%ebp),%eax
 80483f9:	83 f8 ff             	cmp    $0xffffffff,%eax
 80483fc:	75 ef                	jne    80483ed <b+0xd>
 80483fe:	e8 71 ff ff ff       	call   8048374 <a>
 8048403:	e8 8c ff ff ff       	call   8048394 <c>
 8048408:	c9                   	leave  
 8048409:	c3                   	ret    
 804840a:	90                   	nop    
 804840b:	90                   	nop    
 804840c:	90                   	nop    
 804840d:	90                   	nop    
 804840e:	90                   	nop    
 804840f:	90                   	nop    

08048410 <__libc_csu_fini>:
 8048410:	55                   	push   %ebp
 8048411:	89 e5                	mov    %esp,%ebp
 8048413:	5d                   	pop    %ebp
 8048414:	c3                   	ret    
 8048415:	8d 74 26 00          	lea    0x0(%esi),%esi
 8048419:	8d bc 27 00 00 00 00 	lea    0x0(%edi),%edi

08048420 <__libc_csu_init>:
 8048420:	55                   	push   %ebp
 8048421:	89 e5                	mov    %esp,%ebp
 8048423:	57                   	push   %edi
 8048424:	56                   	push   %esi
 8048425:	53                   	push   %ebx
 8048426:	e8 4f 00 00 00       	call   804847a <__i686.get_pc_thunk.bx>
 804842b:	81 c3 a1 11 00 00    	add    $0x11a1,%ebx
 8048431:	83 ec 0c             	sub    $0xc,%esp
 8048434:	e8 3b fe ff ff       	call   8048274 <_init>
 8048439:	8d bb 18 ff ff ff    	lea    -0xe8(%ebx),%edi
 804843f:	8d 83 18 ff ff ff    	lea    -0xe8(%ebx),%eax
 8048445:	29 c7                	sub    %eax,%edi
 8048447:	c1 ff 02             	sar    $0x2,%edi
 804844a:	85 ff                	test   %edi,%edi
 804844c:	74 24                	je     8048472 <__libc_csu_init+0x52>
 804844e:	31 f6                	xor    %esi,%esi
 8048450:	8b 45 10             	mov    0x10(%ebp),%eax
 8048453:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048457:	8b 45 0c             	mov    0xc(%ebp),%eax
 804845a:	89 44 24 04          	mov    %eax,0x4(%esp)
 804845e:	8b 45 08             	mov    0x8(%ebp),%eax
 8048461:	89 04 24             	mov    %eax,(%esp)
 8048464:	ff 94 b3 18 ff ff ff 	call   *-0xe8(%ebx,%esi,4)
 804846b:	83 c6 01             	add    $0x1,%esi
 804846e:	39 f7                	cmp    %esi,%edi
 8048470:	75 de                	jne    8048450 <__libc_csu_init+0x30>
 8048472:	83 c4 0c             	add    $0xc,%esp
 8048475:	5b                   	pop    %ebx
 8048476:	5e                   	pop    %esi
 8048477:	5f                   	pop    %edi
 8048478:	5d                   	pop    %ebp
 8048479:	c3                   	ret    

0804847a <__i686.get_pc_thunk.bx>:
 804847a:	8b 1c 24             	mov    (%esp),%ebx
 804847d:	c3                   	ret    
 804847e:	90                   	nop    
 804847f:	90                   	nop    

08048480 <__do_global_ctors_aux>:
 8048480:	55                   	push   %ebp
 8048481:	89 e5                	mov    %esp,%ebp
 8048483:	53                   	push   %ebx
 8048484:	bb e4 94 04 08       	mov    $0x80494e4,%ebx
 8048489:	83 ec 04             	sub    $0x4,%esp
 804848c:	a1 e4 94 04 08       	mov    0x80494e4,%eax
 8048491:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048494:	74 0c                	je     80484a2 <__do_global_ctors_aux+0x22>
 8048496:	83 eb 04             	sub    $0x4,%ebx
 8048499:	ff d0                	call   *%eax
 804849b:	8b 03                	mov    (%ebx),%eax
 804849d:	83 f8 ff             	cmp    $0xffffffff,%eax
 80484a0:	75 f4                	jne    8048496 <__do_global_ctors_aux+0x16>
 80484a2:	83 c4 04             	add    $0x4,%esp
 80484a5:	5b                   	pop    %ebx
 80484a6:	5d                   	pop    %ebp
 80484a7:	c3                   	ret    
Disassembly of section .fini:

080484a8 <_fini>:
 80484a8:	55                   	push   %ebp
 80484a9:	89 e5                	mov    %esp,%ebp
 80484ab:	53                   	push   %ebx
 80484ac:	83 ec 04             	sub    $0x4,%esp
 80484af:	e8 00 00 00 00       	call   80484b4 <_fini+0xc>
 80484b4:	5b                   	pop    %ebx
 80484b5:	81 c3 18 11 00 00    	add    $0x1118,%ebx
 80484bb:	e8 60 fe ff ff       	call   8048320 <__do_global_dtors_aux>
 80484c0:	59                   	pop    %ecx
 80484c1:	5b                   	pop    %ebx
 80484c2:	c9                   	leave  
 80484c3:	c3                   	ret    
