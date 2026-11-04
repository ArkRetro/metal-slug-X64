4422: 2A FD FB    ld   hl,($FBFD)
4425: 7E          ld   a,(hl)
4426: FE 00       cp   $00
4428: CA E2 46    jp   z,$46E2
442B: FE 01       cp   $01
442D: CA 35 48    jp   z,$4835
4430: FE 02       cp   $02
4432: CA 4B 44    jp   z,$444B
4435: FE 03       cp   $03
4437: CA 42 44    jp   z,$4442
443A: 21 00 00    ld   hl,$0000
443D: E5          push hl
443E: DD E1       pop  ix
4440: A7          and  a
4441: C9          ret
4442: 21 9F FB    ld   hl,$FB9F
4445: 11 FF 03    ld   de,$03FF
4448: C3 F1 49    jp   $49F1
444B: FD 5D       ld   e,iyl
444D: FD 54       ld   d,iyh
444F: D9          exx
4450: AF          xor  a
4451: 57          ld   d,a
4452: 5F          ld   e,a
4453: 6F          ld   l,a
4454: 67          ld   h,a
4455: 06 00       ld   b,$00
4457: DD 21 A9 FA ld   ix,$FAA9
445B: D9          exx
445C: 2A A9 FA    ld   hl,($FAA9)
445F: A7          and  a
4460: ED 52       sbc  hl,de
4462: D9          exx
4463: 20 18       jr   nz,$447D
4465: 3A F9 FB    ld   a,($FBF9)
4468: DD 96 08    sub  (ix+$08)
446B: C2 7D 44    jp   nz,$447D
446E: 3A AE FA    ld   a,($FAAE)
4471: A7          and  a
4472: CA 7D 44    jp   z,$447D
4475: 21 A9 FA    ld   hl,$FAA9
4478: 16 03       ld   d,$03
447A: C3 F1 49    jp   $49F1
447D: FD CB 09 4E bit  1,(iy+$09)
4481: 28 07       jr   z,$448A
4483: DD CB 03 4E bit  1,(ix+$03)
4487: CA C3 44    jp   z,$44C3
448A: 0E 01       ld   c,$01
448C: 3A AE FA    ld   a,($FAAE)
448F: A7          and  a
4490: 20 0A       jr   nz,$449C
4492: 0C          inc  c
4493: 3A B0 FA    ld   a,($FAB0)
4496: DD 96 06    sub  (ix+$06)
4499: 30 01       jr   nc,$449C
449B: 0C          inc  c
449C: 7A          ld   a,d
449D: B9          cp   c
449E: DA B7 44    jp   c,$44B7
44A1: C2 C3 44    jp   nz,$44C3
44A4: 3A AD FA    ld   a,($FAAD)
44A7: BB          cp   e
44A8: 28 06       jr   z,$44B0
44AA: D2 B7 44    jp   nc,$44B7
44AD: C3 C3 44    jp   $44C3
44B0: 3A AF FA    ld   a,($FAAF)
44B3: B8          cp   b
44B4: DA C3 44    jp   c,$44C3
44B7: 51          ld   d,c
44B8: 3A AD FA    ld   a,($FAAD)
44BB: 5F          ld   e,a
44BC: 3A AF FA    ld   a,($FAAF)
44BF: 47          ld   b,a
44C0: 21 A9 FA    ld   hl,$FAA9
44C3: DD 21 D2 FA ld   ix,$FAD2
44C7: D9          exx
44C8: 2A D2 FA    ld   hl,($FAD2)
44CB: A7          and  a
44CC: ED 52       sbc  hl,de
44CE: D9          exx
44CF: 20 18       jr   nz,$44E9
44D1: 3A F9 FB    ld   a,($FBF9)
44D4: DD 96 08    sub  (ix+$08)
44D7: C2 E9 44    jp   nz,$44E9
44DA: 3A D7 FA    ld   a,($FAD7)
44DD: A7          and  a
44DE: CA E9 44    jp   z,$44E9
44E1: 21 D2 FA    ld   hl,$FAD2
44E4: 16 03       ld   d,$03
44E6: C3 F1 49    jp   $49F1
44E9: FD CB 09 4E bit  1,(iy+$09)
44ED: 28 07       jr   z,$44F6
44EF: DD CB 03 4E bit  1,(ix+$03)
44F3: CA 2F 45    jp   z,$452F
44F6: 0E 01       ld   c,$01
44F8: 3A D7 FA    ld   a,($FAD7)
44FB: A7          and  a
44FC: 20 0A       jr   nz,$4508
44FE: 0C          inc  c
44FF: 3A D9 FA    ld   a,($FAD9)
4502: DD 96 06    sub  (ix+$06)
4505: 30 01       jr   nc,$4508
4507: 0C          inc  c
4508: 7A          ld   a,d
4509: B9          cp   c
450A: DA 23 45    jp   c,$4523
450D: C2 2F 45    jp   nz,$452F
4510: 3A D6 FA    ld   a,($FAD6)
4513: BB          cp   e
4514: 28 06       jr   z,$451C
4516: D2 23 45    jp   nc,$4523
4519: C3 2F 45    jp   $452F
451C: 3A D8 FA    ld   a,($FAD8)
451F: B8          cp   b
4520: DA 2F 45    jp   c,$452F
4523: 51          ld   d,c
4524: 3A D6 FA    ld   a,($FAD6)
4527: 5F          ld   e,a
4528: 3A D8 FA    ld   a,($FAD8)
452B: 47          ld   b,a
452C: 21 D2 FA    ld   hl,$FAD2
452F: DD 21 FB FA ld   ix,$FAFB
4533: D9          exx
4534: 2A FB FA    ld   hl,($FAFB)
4537: A7          and  a
4538: ED 52       sbc  hl,de
453A: D9          exx
453B: 20 18       jr   nz,$4555
453D: 3A F9 FB    ld   a,($FBF9)
4540: DD 96 08    sub  (ix+$08)
4543: C2 55 45    jp   nz,$4555
4546: 3A 00 FB    ld   a,($FB00)
4549: A7          and  a
454A: CA 55 45    jp   z,$4555
454D: 21 FB FA    ld   hl,$FAFB
4550: 16 03       ld   d,$03
4552: C3 F1 49    jp   $49F1
4555: FD CB 09 4E bit  1,(iy+$09)
4559: 28 07       jr   z,$4562
455B: DD CB 03 4E bit  1,(ix+$03)
455F: CA 9B 45    jp   z,$459B
4562: 0E 01       ld   c,$01
4564: 3A 00 FB    ld   a,($FB00)
4567: A7          and  a
4568: 20 0A       jr   nz,$4574
456A: 0C          inc  c
456B: 3A 02 FB    ld   a,($FB02)
456E: DD 96 06    sub  (ix+$06)
4571: 30 01       jr   nc,$4574
4573: 0C          inc  c
4574: 7A          ld   a,d
4575: B9          cp   c
4576: DA 8F 45    jp   c,$458F
4579: C2 9B 45    jp   nz,$459B
457C: 3A FF FA    ld   a,($FAFF)
457F: BB          cp   e
4580: 28 06       jr   z,$4588
4582: D2 8F 45    jp   nc,$458F
4585: C3 9B 45    jp   $459B
4588: 3A 01 FB    ld   a,($FB01)
458B: B8          cp   b
458C: DA 9B 45    jp   c,$459B
458F: 51          ld   d,c
4590: 3A FF FA    ld   a,($FAFF)
4593: 5F          ld   e,a
4594: 3A 01 FB    ld   a,($FB01)
4597: 47          ld   b,a
4598: 21 FB FA    ld   hl,$FAFB
459B: DD 21 24 FB ld   ix,$FB24
459F: D9          exx
45A0: 2A 24 FB    ld   hl,($FB24)
45A3: A7          and  a
45A4: ED 52       sbc  hl,de
45A6: D9          exx
45A7: 20 18       jr   nz,$45C1
45A9: 3A F9 FB    ld   a,($FBF9)
45AC: DD 96 08    sub  (ix+$08)
45AF: C2 C1 45    jp   nz,$45C1
45B2: 3A 29 FB    ld   a,($FB29)
45B5: A7          and  a
45B6: CA C1 45    jp   z,$45C1
45B9: 21 24 FB    ld   hl,$FB24
45BC: 16 03       ld   d,$03
45BE: C3 F1 49    jp   $49F1
45C1: FD CB 09 4E bit  1,(iy+$09)
45C5: 28 07       jr   z,$45CE
45C7: DD CB 03 4E bit  1,(ix+$03)
45CB: CA 07 46    jp   z,$4607
45CE: 0E 01       ld   c,$01
45D0: 3A 29 FB    ld   a,($FB29)
45D3: A7          and  a
45D4: 20 0A       jr   nz,$45E0
45D6: 0C          inc  c
45D7: 3A 2B FB    ld   a,($FB2B)
45DA: DD 96 06    sub  (ix+$06)
45DD: 30 01       jr   nc,$45E0
45DF: 0C          inc  c
45E0: 7A          ld   a,d
45E1: B9          cp   c
45E2: DA FB 45    jp   c,$45FB
45E5: C2 07 46    jp   nz,$4607
45E8: 3A 28 FB    ld   a,($FB28)
45EB: BB          cp   e
45EC: 28 06       jr   z,$45F4
45EE: D2 FB 45    jp   nc,$45FB
45F1: C3 07 46    jp   $4607
45F4: 3A 2A FB    ld   a,($FB2A)
45F7: B8          cp   b
45F8: DA 07 46    jp   c,$4607
45FB: 51          ld   d,c
45FC: 3A 28 FB    ld   a,($FB28)
45FF: 5F          ld   e,a
4600: 3A 2A FB    ld   a,($FB2A)
4603: 47          ld   b,a
4604: 21 24 FB    ld   hl,$FB24
4607: DD 21 4D FB ld   ix,$FB4D
460B: D9          exx
460C: 2A 4D FB    ld   hl,($FB4D)
460F: A7          and  a
4610: ED 52       sbc  hl,de
4612: D9          exx
4613: 20 18       jr   nz,$462D
4615: 3A F9 FB    ld   a,($FBF9)
4618: DD 96 08    sub  (ix+$08)
461B: C2 2D 46    jp   nz,$462D
461E: 3A 52 FB    ld   a,($FB52)
4621: A7          and  a
4622: CA 2D 46    jp   z,$462D
4625: 21 4D FB    ld   hl,$FB4D
4628: 16 03       ld   d,$03
462A: C3 F1 49    jp   $49F1
462D: FD CB 09 4E bit  1,(iy+$09)
4631: 28 07       jr   z,$463A
4633: DD CB 03 4E bit  1,(ix+$03)
4637: CA 73 46    jp   z,$4673
463A: 0E 01       ld   c,$01
463C: 3A 52 FB    ld   a,($FB52)
463F: A7          and  a
4640: 20 0A       jr   nz,$464C
4642: 0C          inc  c
4643: 3A 54 FB    ld   a,($FB54)
4646: DD 96 06    sub  (ix+$06)
4649: 30 01       jr   nc,$464C
464B: 0C          inc  c
464C: 7A          ld   a,d
464D: B9          cp   c
464E: DA 67 46    jp   c,$4667
4651: C2 73 46    jp   nz,$4673
4654: 3A 51 FB    ld   a,($FB51)
4657: BB          cp   e
4658: 28 06       jr   z,$4660
465A: D2 67 46    jp   nc,$4667
465D: C3 73 46    jp   $4673
4660: 3A 53 FB    ld   a,($FB53)
4663: B8          cp   b
4664: DA 73 46    jp   c,$4673
4667: 51          ld   d,c
4668: 3A 51 FB    ld   a,($FB51)
466B: 5F          ld   e,a
466C: 3A 53 FB    ld   a,($FB53)
466F: 47          ld   b,a
4670: 21 4D FB    ld   hl,$FB4D
4673: DD 21 76 FB ld   ix,$FB76
4677: D9          exx
4678: 2A 76 FB    ld   hl,($FB76)
467B: A7          and  a
467C: ED 52       sbc  hl,de
467E: D9          exx
467F: 20 18       jr   nz,$4699
4681: 3A F9 FB    ld   a,($FBF9)
4684: DD 96 08    sub  (ix+$08)
4687: C2 99 46    jp   nz,$4699
468A: 3A 7B FB    ld   a,($FB7B)
468D: A7          and  a
468E: CA 99 46    jp   z,$4699
4691: 21 76 FB    ld   hl,$FB76
4694: 16 03       ld   d,$03
4696: C3 F1 49    jp   $49F1
4699: FD CB 09 4E bit  1,(iy+$09)
469D: 28 07       jr   z,$46A6
469F: DD CB 03 4E bit  1,(ix+$03)
46A3: CA DF 46    jp   z,$46DF
46A6: 0E 01       ld   c,$01
46A8: 3A 7B FB    ld   a,($FB7B)
46AB: A7          and  a
46AC: 20 0A       jr   nz,$46B8
46AE: 0C          inc  c
46AF: 3A 7D FB    ld   a,($FB7D)
46B2: DD 96 06    sub  (ix+$06)
46B5: 30 01       jr   nc,$46B8
46B7: 0C          inc  c
46B8: 7A          ld   a,d
46B9: B9          cp   c
46BA: DA D3 46    jp   c,$46D3
46BD: C2 DF 46    jp   nz,$46DF
46C0: 3A 7A FB    ld   a,($FB7A)
46C3: BB          cp   e
46C4: 28 06       jr   z,$46CC
46C6: D2 D3 46    jp   nc,$46D3
46C9: C3 DF 46    jp   $46DF
46CC: 3A 7C FB    ld   a,($FB7C)
46CF: B8          cp   b
46D0: DA DF 46    jp   c,$46DF
46D3: 51          ld   d,c
46D4: 3A 7A FB    ld   a,($FB7A)
46D7: 5F          ld   e,a
46D8: 3A 7C FB    ld   a,($FB7C)
46DB: 47          ld   b,a
46DC: 21 76 FB    ld   hl,$FB76
46DF: C3 F1 49    jp   $49F1
46E2: FD 5D       ld   e,iyl
46E4: FD 54       ld   d,iyh
46E6: D9          exx
46E7: AF          xor  a
46E8: 57          ld   d,a
46E9: 5F          ld   e,a
46EA: 6F          ld   l,a
46EB: 67          ld   h,a
46EC: 06 00       ld   b,$00
46EE: DD 21 1C F8 ld   ix,$F81C
46F2: D9          exx
46F3: 2A 1C F8    ld   hl,($F81C)
46F6: A7          and  a
46F7: ED 52       sbc  hl,de
46F9: D9          exx
46FA: 20 18       jr   nz,$4714
46FC: 3A F9 FB    ld   a,($FBF9)
46FF: DD 96 08    sub  (ix+$08)
4702: C2 14 47    jp   nz,$4714
4705: 3A 21 F8    ld   a,($F821)
4708: A7          and  a
4709: CA 14 47    jp   z,$4714
470C: 21 1C F8    ld   hl,$F81C
470F: 16 03       ld   d,$03
4711: C3 F1 49    jp   $49F1
4714: FD CB 09 4E bit  1,(iy+$09)
4718: 28 07       jr   z,$4721
471A: DD CB 03 4E bit  1,(ix+$03)
471E: CA 5A 47    jp   z,$475A
4721: 0E 01       ld   c,$01
4723: 3A 21 F8    ld   a,($F821)
4726: A7          and  a
4727: 20 0A       jr   nz,$4733
4729: 0C          inc  c
472A: 3A 23 F8    ld   a,($F823)
472D: DD 96 06    sub  (ix+$06)
4730: 30 01       jr   nc,$4733
4732: 0C          inc  c
4733: 7A          ld   a,d
4734: B9          cp   c
4735: DA 4E 47    jp   c,$474E
4738: C2 5A 47    jp   nz,$475A
473B: 3A 20 F8    ld   a,($F820)
473E: BB          cp   e
473F: 28 06       jr   z,$4747
4741: D2 4E 47    jp   nc,$474E
4744: C3 5A 47    jp   $475A
4747: 3A 22 F8    ld   a,($F822)
474A: B8          cp   b
474B: DA 5A 47    jp   c,$475A
474E: 51          ld   d,c
474F: 3A 20 F8    ld   a,($F820)
4752: 5F          ld   e,a
4753: 3A 22 F8    ld   a,($F822)
4756: 47          ld   b,a
4757: 21 1C F8    ld   hl,$F81C
475A: DD 21 69 F8 ld   ix,$F869
475E: D9          exx
475F: 2A 69 F8    ld   hl,($F869)
4762: A7          and  a
4763: ED 52       sbc  hl,de
4765: D9          exx
4766: 20 18       jr   nz,$4780
4768: 3A F9 FB    ld   a,($FBF9)
476B: DD 96 08    sub  (ix+$08)
476E: C2 80 47    jp   nz,$4780
4771: 3A 6E F8    ld   a,($F86E)
4774: A7          and  a
4775: CA 80 47    jp   z,$4780
4778: 21 69 F8    ld   hl,$F869
477B: 16 03       ld   d,$03
477D: C3 F1 49    jp   $49F1
4780: FD CB 09 4E bit  1,(iy+$09)
4784: 28 07       jr   z,$478D
4786: DD CB 03 4E bit  1,(ix+$03)
478A: CA C6 47    jp   z,$47C6
478D: 0E 01       ld   c,$01
478F: 3A 6E F8    ld   a,($F86E)
4792: A7          and  a
4793: 20 0A       jr   nz,$479F
4795: 0C          inc  c
4796: 3A 70 F8    ld   a,($F870)
4799: DD 96 06    sub  (ix+$06)
479C: 30 01       jr   nc,$479F
479E: 0C          inc  c
479F: 7A          ld   a,d
47A0: B9          cp   c
47A1: DA BA 47    jp   c,$47BA
47A4: C2 C6 47    jp   nz,$47C6
47A7: 3A 6D F8    ld   a,($F86D)
47AA: BB          cp   e
47AB: 28 06       jr   z,$47B3
47AD: D2 BA 47    jp   nc,$47BA
47B0: C3 C6 47    jp   $47C6
47B3: 3A 6F F8    ld   a,($F86F)
47B6: B8          cp   b
47B7: DA C6 47    jp   c,$47C6
47BA: 51          ld   d,c
47BB: 3A 6D F8    ld   a,($F86D)
47BE: 5F          ld   e,a
47BF: 3A 6F F8    ld   a,($F86F)
47C2: 47          ld   b,a
47C3: 21 69 F8    ld   hl,$F869
47C6: DD 21 B6 F8 ld   ix,$F8B6
47CA: D9          exx
47CB: 2A B6 F8    ld   hl,($F8B6)
47CE: A7          and  a
47CF: ED 52       sbc  hl,de
47D1: D9          exx
47D2: 20 18       jr   nz,$47EC
47D4: 3A F9 FB    ld   a,($FBF9)
47D7: DD 96 08    sub  (ix+$08)
47DA: C2 EC 47    jp   nz,$47EC
47DD: 3A BB F8    ld   a,($F8BB)
47E0: A7          and  a
47E1: CA EC 47    jp   z,$47EC
47E4: 21 B6 F8    ld   hl,$F8B6
47E7: 16 03       ld   d,$03
47E9: C3 F1 49    jp   $49F1
47EC: FD CB 09 4E bit  1,(iy+$09)
47F0: 28 07       jr   z,$47F9
47F2: DD CB 03 4E bit  1,(ix+$03)
47F6: CA 32 48    jp   z,$4832
47F9: 0E 01       ld   c,$01
47FB: 3A BB F8    ld   a,($F8BB)
47FE: A7          and  a
47FF: 20 0A       jr   nz,$480B
4801: 0C          inc  c
4802: 3A BD F8    ld   a,($F8BD)
4805: DD 96 06    sub  (ix+$06)
4808: 30 01       jr   nc,$480B
480A: 0C          inc  c
480B: 7A          ld   a,d
480C: B9          cp   c
480D: DA 26 48    jp   c,$4826
4810: C2 32 48    jp   nz,$4832
4813: 3A BA F8    ld   a,($F8BA)
4816: BB          cp   e
4817: 28 06       jr   z,$481F
4819: D2 26 48    jp   nc,$4826
481C: C3 32 48    jp   $4832
481F: 3A BC F8    ld   a,($F8BC)
4822: B8          cp   b
4823: DA 32 48    jp   c,$4832
4826: 51          ld   d,c
4827: 3A BA F8    ld   a,($F8BA)
482A: 5F          ld   e,a
482B: 3A BC F8    ld   a,($F8BC)
482E: 47          ld   b,a
482F: 21 B6 F8    ld   hl,$F8B6
4832: C3 F1 49    jp   $49F1
4835: FD 5D       ld   e,iyl
4837: FD 54       ld   d,iyh
4839: D9          exx
483A: AF          xor  a
483B: 57          ld   d,a
483C: 5F          ld   e,a
483D: 6F          ld   l,a
483E: 67          ld   h,a
483F: 06 00       ld   b,$00
4841: DD 21 07 F9 ld   ix,$F907
4845: D9          exx
4846: 2A 07 F9    ld   hl,($F907)
4849: A7          and  a
484A: ED 52       sbc  hl,de
484C: D9          exx
484D: 20 18       jr   nz,$4867
484F: 3A F9 FB    ld   a,($FBF9)
4852: DD 96 08    sub  (ix+$08)
4855: C2 67 48    jp   nz,$4867
4858: 3A 0C F9    ld   a,($F90C)
485B: A7          and  a
485C: CA 67 48    jp   z,$4867
485F: 21 07 F9    ld   hl,$F907
4862: 16 03       ld   d,$03
4864: C3 F1 49    jp   $49F1
4867: FD CB 09 4E bit  1,(iy+$09)
486B: 28 07       jr   z,$4874
486D: DD CB 03 4E bit  1,(ix+$03)
4871: CA AD 48    jp   z,$48AD
4874: 0E 01       ld   c,$01
4876: 3A 0C F9    ld   a,($F90C)
4879: A7          and  a
487A: 20 0A       jr   nz,$4886
487C: 0C          inc  c
487D: 3A 0E F9    ld   a,($F90E)
4880: DD 96 06    sub  (ix+$06)
4883: 30 01       jr   nc,$4886
4885: 0C          inc  c
4886: 7A          ld   a,d
4887: B9          cp   c
4888: DA A1 48    jp   c,$48A1
488B: C2 AD 48    jp   nz,$48AD
488E: 3A 0B F9    ld   a,($F90B)
4891: BB          cp   e
4892: 28 06       jr   z,$489A
4894: D2 A1 48    jp   nc,$48A1
4897: C3 AD 48    jp   $48AD
489A: 3A 0D F9    ld   a,($F90D)
489D: B8          cp   b
489E: DA AD 48    jp   c,$48AD
48A1: 51          ld   d,c
48A2: 3A 0B F9    ld   a,($F90B)
48A5: 5F          ld   e,a
48A6: 3A 0D F9    ld   a,($F90D)
48A9: 47          ld   b,a
48AA: 21 07 F9    ld   hl,$F907
48AD: DD 21 6F F9 ld   ix,$F96F
48B1: D9          exx
48B2: 2A 6F F9    ld   hl,($F96F)
48B5: A7          and  a
48B6: ED 52       sbc  hl,de
48B8: D9          exx
48B9: 20 18       jr   nz,$48D3
48BB: 3A F9 FB    ld   a,($FBF9)
48BE: DD 96 08    sub  (ix+$08)
48C1: C2 D3 48    jp   nz,$48D3
48C4: 3A 74 F9    ld   a,($F974)
48C7: A7          and  a
48C8: CA D3 48    jp   z,$48D3
48CB: 21 6F F9    ld   hl,$F96F
48CE: 16 03       ld   d,$03
48D0: C3 F1 49    jp   $49F1
48D3: FD CB 09 4E bit  1,(iy+$09)
48D7: 28 07       jr   z,$48E0
48D9: DD CB 03 4E bit  1,(ix+$03)
48DD: CA 19 49    jp   z,$4919
48E0: 0E 01       ld   c,$01
48E2: 3A 74 F9    ld   a,($F974)
48E5: A7          and  a
48E6: 20 0A       jr   nz,$48F2
48E8: 0C          inc  c
48E9: 3A 76 F9    ld   a,($F976)
48EC: DD 96 06    sub  (ix+$06)
48EF: 30 01       jr   nc,$48F2
48F1: 0C          inc  c
48F2: 7A          ld   a,d
48F3: B9          cp   c
48F4: DA 0D 49    jp   c,$490D
48F7: C2 19 49    jp   nz,$4919
48FA: 3A 73 F9    ld   a,($F973)
48FD: BB          cp   e
48FE: 28 06       jr   z,$4906
4900: D2 0D 49    jp   nc,$490D
4903: C3 19 49    jp   $4919
4906: 3A 75 F9    ld   a,($F975)
4909: B8          cp   b
490A: DA 19 49    jp   c,$4919
490D: 51          ld   d,c
490E: 3A 73 F9    ld   a,($F973)
4911: 5F          ld   e,a
4912: 3A 75 F9    ld   a,($F975)
4915: 47          ld   b,a
4916: 21 6F F9    ld   hl,$F96F
4919: DD 21 D7 F9 ld   ix,$F9D7
491D: D9          exx
491E: 2A D7 F9    ld   hl,($F9D7)
4921: A7          and  a
4922: ED 52       sbc  hl,de
4924: D9          exx
4925: 20 18       jr   nz,$493F
4927: 3A F9 FB    ld   a,($FBF9)
492A: DD 96 08    sub  (ix+$08)
492D: C2 3F 49    jp   nz,$493F
4930: 3A DC F9    ld   a,($F9DC)
4933: A7          and  a
4934: CA 3F 49    jp   z,$493F
4937: 21 D7 F9    ld   hl,$F9D7
493A: 16 03       ld   d,$03
493C: C3 F1 49    jp   $49F1
493F: FD CB 09 4E bit  1,(iy+$09)
4943: 28 07       jr   z,$494C
4945: DD CB 03 4E bit  1,(ix+$03)
4949: CA 85 49    jp   z,$4985
494C: 0E 01       ld   c,$01
494E: 3A DC F9    ld   a,($F9DC)
4951: A7          and  a
4952: 20 0A       jr   nz,$495E
4954: 0C          inc  c
4955: 3A DE F9    ld   a,($F9DE)
4958: DD 96 06    sub  (ix+$06)
495B: 30 01       jr   nc,$495E
495D: 0C          inc  c
495E: 7A          ld   a,d
495F: B9          cp   c
4960: DA 79 49    jp   c,$4979
4963: C2 85 49    jp   nz,$4985
4966: 3A DB F9    ld   a,($F9DB)
4969: BB          cp   e
496A: 28 06       jr   z,$4972
496C: D2 79 49    jp   nc,$4979
496F: C3 85 49    jp   $4985
4972: 3A DD F9    ld   a,($F9DD)
4975: B8          cp   b
4976: DA 85 49    jp   c,$4985
4979: 51          ld   d,c
497A: 3A DB F9    ld   a,($F9DB)
497D: 5F          ld   e,a
497E: 3A DD F9    ld   a,($F9DD)
4981: 47          ld   b,a
4982: 21 D7 F9    ld   hl,$F9D7
4985: DD 21 3F FA ld   ix,$FA3F
4989: D9          exx
498A: 2A 3F FA    ld   hl,($FA3F)
498D: A7          and  a
498E: ED 52       sbc  hl,de
4990: D9          exx
4991: 20 18       jr   nz,$49AB
4993: 3A F9 FB    ld   a,($FBF9)
4996: DD 96 08    sub  (ix+$08)
4999: C2 AB 49    jp   nz,$49AB
499C: 3A 44 FA    ld   a,($FA44)
499F: A7          and  a
49A0: CA AB 49    jp   z,$49AB
49A3: 21 3F FA    ld   hl,$FA3F
49A6: 16 03       ld   d,$03
49A8: C3 F1 49    jp   $49F1
49AB: FD CB 09 4E bit  1,(iy+$09)
49AF: 28 07       jr   z,$49B8
49B1: DD CB 03 4E bit  1,(ix+$03)
49B5: CA F1 49    jp   z,$49F1
49B8: 0E 01       ld   c,$01
49BA: 3A 44 FA    ld   a,($FA44)
49BD: A7          and  a
49BE: 20 0A       jr   nz,$49CA
49C0: 0C          inc  c
49C1: 3A 46 FA    ld   a,($FA46)
49C4: DD 96 06    sub  (ix+$06)
49C7: 30 01       jr   nc,$49CA
49C9: 0C          inc  c
49CA: 7A          ld   a,d
49CB: B9          cp   c
49CC: DA E5 49    jp   c,$49E5
49CF: C2 F1 49    jp   nz,$49F1
49D2: 3A 43 FA    ld   a,($FA43)
49D5: BB          cp   e
49D6: 28 06       jr   z,$49DE
49D8: D2 E5 49    jp   nc,$49E5
49DB: C3 F1 49    jp   $49F1
49DE: 3A 45 FA    ld   a,($FA45)
49E1: B8          cp   b
49E2: DA F1 49    jp   c,$49F1
49E5: 51          ld   d,c
49E6: 3A 43 FA    ld   a,($FA43)
49E9: 5F          ld   e,a
49EA: 3A 45 FA    ld   a,($FA45)
49ED: 47          ld   b,a
49EE: 21 3F FA    ld   hl,$FA3F
49F1: EB          ex   de,hl
49F2: DD 6B       ld   ixl,e
49F4: DD 62       ld   ixh,d
49F6: 7C          ld   a,h
49F7: A7          and  a
49F8: C8          ret  z
49F9: FE 02       cp   $02
49FB: 30 0A       jr   nc,$4A07
49FD: 7D          ld   a,l
49FE: FD BE 02    cp   (iy+$02)
4A01: 3F          ccf
4A02: D0          ret  nc
4A03: DD 36 05 00 ld   (ix+$05),$00
4A07: FD 6E 00    ld   l,(iy+$00)
4A0A: FD 66 01    ld   h,(iy+$01)
4A0D: FD 73 00    ld   (iy+$00),e
4A10: FD 72 01    ld   (iy+$01),d
4A13: FD 5D       ld   e,iyl
4A15: FD 54       ld   d,iyh
4A17: DD 73 00    ld   (ix+$00),e
4A1A: DD 72 01    ld   (ix+$01),d
4A1D: 7D          ld   a,l
4A1E: B4          or   h
4A1F: CA 28 4A    jp   z,$4A28
4A22: 11 04 00    ld   de,$0004
4A25: 19          add  hl,de
4A26: 36 FF       ld   (hl),$FF
4A28: 37          scf
4A29: C9          ret
4A2A: CD 84 4B    call $4B84
4A2D: 22 FD FB    ld   ($FBFD),hl
4A30: 78          ld   a,b
4A31: 32 FF FB    ld   ($FBFF),a
4A34: FD CB 09 46 bit  0,(iy+$09)
4A38: 28 1D       jr   z,$4A57
4A3A: CD 09 4B    call $4B09
4A3D: 38 0E       jr   c,$4A4D
4A3F: FD CB 09 4E bit  1,(iy+$09)
4A43: 20 12       jr   nz,$4A57
4A45: FD CB 09 6E bit  5,(iy+$09)
4A49: 28 0C       jr   z,$4A57
4A4B: A7          and  a
4A4C: C9          ret
4A4D: 2A FD FB    ld   hl,($FBFD)
4A50: 7E          ld   a,(hl)
4A51: DD BE 02    cp   (ix+$02)
4A54: CA 5B 4A    jp   z,$4A5B
4A57: CD 22 44    call $4422
4A5A: D0          ret  nc
4A5B: FD CB 09 EE set  5,(iy+$09)
4A5F: FD 7E 02    ld   a,(iy+$02)
4A62: DD 77 04    ld   (ix+$04),a
4A65: 3A F9 FB    ld   a,($FBF9)
4A68: DD 77 08    ld   (ix+$08),a
4A6B: 3A FC FB    ld   a,($FBFC)
4A6E: DD 77 0B    ld   (ix+$0b),a
4A71: 3A FA FB    ld   a,($FBFA)
4A74: DD 77 07    ld   (ix+$07),a
4A77: DD 36 06 00 ld   (ix+$06),$00
4A7B: DD 46 05    ld   b,(ix+$05)
4A7E: 3A FB FB    ld   a,($FBFB)
4A81: D6 01       sub  $01
4A83: CE 01       adc  a,$01
4A85: DD 77 05    ld   (ix+$05),a
4A88: FD 7E 09    ld   a,(iy+$09)
4A8B: DD 77 03    ld   (ix+$03),a
4A8E: 04          inc  b
4A8F: 05          dec  b
4A90: C2 EB 4A    jp   nz,$4AEB
4A93: FD 5E 0A    ld   e,(iy+$0a)
4A96: FD 56 0B    ld   d,(iy+$0b)
4A99: DD 6E 0C    ld   l,(ix+$0c)
4A9C: DD 66 0D    ld   h,(ix+$0d)
4A9F: A7          and  a
4AA0: ED 52       sbc  hl,de
4AA2: CA CF 4A    jp   z,$4ACF
4AA5: DD 73 0C    ld   (ix+$0c),e
4AA8: DD 72 0D    ld   (ix+$0d),d
4AAB: 2A FD FB    ld   hl,($FBFD)
4AAE: 3A FF FB    ld   a,($FBFF)
4AB1: 47          ld   b,a
4AB2: D9          exx
4AB3: 21 CF 4A    ld   hl,$4ACF
4AB6: E5          push hl
4AB7: DD 7E 02    ld   a,(ix+$02)
4ABA: 87          add  a,a
4ABB: 6F          ld   l,a
4ABC: 26 00       ld   h,$00
4ABE: 11 C7 4A    ld   de,$4AC7
4AC1: 19          add  hl,de
4AC2: 5E          ld   e,(hl)
4AC3: 23          inc  hl
4AC4: 56          ld   d,(hl)
4AC5: EB          ex   de,hl
4AC6: E9          jp   (hl)
