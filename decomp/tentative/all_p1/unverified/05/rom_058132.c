/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058132.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000582dc: Changing call to branch */
/* WARNING: Possible PIC construction at 0x00058358: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x0005835e) */
/* WARNING: Removing unreachable block (ram,0x000583a4) */
/* WARNING: Removing unreachable block (ram,0x000583e8) */
/* WARNING: Removing unreachable block (ram,0x000583b6) */
/* WARNING: Removing unreachable block (ram,0x000583d0) */
/* WARNING: Removing unreachable block (ram,0x000583d4) */
/* WARNING: Removing unreachable block (ram,0x000583de) */
/* WARNING: Removing unreachable block (ram,0x000583e2) */
/* WARNING: Removing unreachable block (ram,0x00004188) */
/* WARNING: Removing unreachable block (ram,0x0000424a) */
/* WARNING: Removing unreachable block (ram,0x00004240) */
/* WARNING: Removing unreachable block (ram,0x00058372) */
/* WARNING: Removing unreachable block (ram,0x00058380) */
/* WARNING: Removing unreachable block (ram,0x00058384) */
/* WARNING: Removing unreachable block (ram,0x00058392) */
/* WARNING: Removing unreachable block (ram,0x0005838e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_58132(void)

{
  short sVar1;
  undefined4 uVar2;
  ushort uVar4;
  uint uVar3;
  undefined2 extraout_D1u;
  byte bVar6;
  ushort uVar5;
  byte bVar7;
  short sVar8;
  uint uVar9;
  char cVar10;
  char cVar11;
  int extraout_A0;
  byte *pbVar12;
  int iVar13;
  undefined4 *puVar14;
  short *psVar15;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar16;
  ulonglong uVar17;

  *(undefined1 *)(unaff_A6 + 0x21) = 0;
  iVar13 = *(int *)((int)unaff_A6 + 0x72);
  if (iVar13 == 0x10c46e) {
    FUN_00004464[unaff_A5 + 4] = (code)0xff;
  }
  if (iVar13 == 0x10c476) {
    *(undefined1 *)(unaff_A5 + 0x4469) = 0xff;
  }
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  candidate_57a98();
  candidate_58530();
  *(undefined2 *)(unaff_A6 + 0x1f) = 0xf;
  uVar3 = candidate_3cab4();
  *(undefined2 *)(unaff_A6 + 0x20) = 600;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 300;
  *(undefined1 *)((int)unaff_A6 + 0x85) = 0;
  *unaff_A6 = 0x58198;
  if (DAT_0010c2e6 == '\0') {
    return uVar3;
  }
  candidate_58592();
  bVar16 = false;
  if ((*(byte *)(extraout_A0 + 3) & 0x10) == 0) {
    if ((*(byte *)(extraout_A0 + 3) & 0x20) == 0) {
      uVar3 = candidate_58554();
      if ((uVar3 & 8) == 0) {
        if ((uVar3 & 4) != 0) {
          candidate_584f2();
        }
      }
      else {
        candidate_584ae();
      }
    }
    else {
      candidate_5845a();
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x85) = 0xff;
    candidate_583ea();
    if (bVar16) goto LAB_000582f2;
  }
  sVar1 = *(short *)(unaff_A6 + 0x1f) + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar1;
  if (sVar1 == 0) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0xf;
  }
  sVar1 = *(short *)(unaff_A6 + 0x20) + -1;
  *(short *)(unaff_A6 + 0x20) = sVar1;
  if ((sVar1 != 0) &&
     (sVar1 = *(short *)((int)unaff_A6 + 0x82) + -1, *(short *)((int)unaff_A6 + 0x82) = sVar1,
     sVar1 != 0)) {
    if (*(char *)(unaff_A6 + 0x21) == '\0') {
      candidate_57b44();
    }
    candidate_3dd96();
    candidate_3dd96();
    candidate_3dd96();
    candidate_3dd96();
    uVar17 = (ulonglong)CONCAT24(*(short *)(unaff_A6 + 0x1c) + 0xa0,CONCAT22(extraout_D1u,0x441));
    sVar1 = 0;
    do {
      uVar3 = (uint)(uVar17 >> 0x20);
      uVar2 = (undefined4)uVar17;
      sVar8 = (short)(uVar17 >> 0x20);
      if ((sVar1 == *(short *)(unaff_A6 + 0x1e)) && (*(short *)(unaff_A6 + 0x1f) < 8)) {
        candidate_3dd96();
        uVar17 = (ulonglong)CONCAT24(sVar8 + 0x40,uVar2);
      }
      else {
        if (sVar1 == 3) {
          if (*(short *)(unaff_A6 + 0x1e) != 3) {
            sVar1 = 2;
            goto SUB_0003cf12;
          }
          pbVar12 = (byte *)0x585f0;
          bVar6 = (byte)uVar17;
          uVar5 = (ushort)uVar17;
          if (bVar6 < 0x10) {
            _DAT_003c0000 =
                 CONCAT22(0x8200,(short)CONCAT31((int3)(CONCAT22(sVar8,(short)CONCAT31((int3)(
                                                  CONCAT22(sVar8,uVar5) >> 8),bVar6 << 4) << 4) >> 8
                                                  ),bRam000585ef));
            return (uint)(ushort)(sVar8 + 0x20);
          }
          if (cRam000585f0 == -0x22) {
            if (bRam000585ef == 0xb3) {
              pbVar12 = (byte *)0x585f1;
              bVar7 = 0xe0;
            }
            else if ((bRam000585ef < 0xb6) || (0xc4 < bRam000585ef)) {
              if ((bRam000585ef < 0xca) || (0xce < bRam000585ef)) goto LAB_0003ddfe;
              pbVar12 = (byte *)0x585f1;
              bVar7 = (bRam000585ef + 0x36) * '\x02' - 0x10;
            }
            else {
              pbVar12 = (byte *)0x585f1;
              bVar7 = bRam000585ef + 0x2b;
            }
          }
          else {
LAB_0003ddfe:
            bVar7 = bRam000585ef;
            if (((cRam000585f0 == -0x21) && (0xc9 < bRam000585ef)) && (bRam000585ef < 0xcf)) {
              pbVar12 = (byte *)0x585f1;
              bVar7 = (bRam000585ef + 0x36) * '\x02' - 0xf;
            }
          }
          if (bVar6 == 0x30) {
            uVar3 = candidate_3cf9a(uVar2,bRam000585ef);
            return uVar3;
          }
          if (bVar6 == 0x61) {
            _DAT_003c0000 =
                 CONCAT31((int3)(CONCAT22(sVar8,(short)CONCAT31((int3)(CONCAT22(sVar8,uVar5) >> 8),
                                                                0xa0) << 4) >> 8),
                          (bVar7 & 0xf) + (bVar7 & 0xf0) * '\x02') + 0x10010;
            return (uint)(ushort)((short)(_DAT_003c0000 >> 0x10) + 0x1f);
          }
          if (bVar6 == 0x48) {
            iVar13 = 0x3d596;
          }
          else {
            if (bVar6 != 0x4b) {
              if (bVar6 != 0x41) {
                if (bVar6 == 0x31) {
                  _DAT_003c0000 =
                       CONCAT31((int3)(CONCAT22(sVar8,(short)CONCAT31((int3)(CONCAT22(sVar8,uVar5)
                                                                            >> 8),0x40) << 4) >> 8),
                                bVar7 + 0x80) + 0x20020;
                  return (uint)(ushort)((short)(_DAT_003c0000 >> 0x10) + 0x1e);
                }
                if (bVar6 == 0x68) {
                  iVar13 = 0x3db96;
                }
                else {
                  if (bVar6 != 0x6b) {
                    if (bVar6 != 0x58) {
                      if ((bVar6 != 0x32) || (bVar7 != 0x40)) {
                        uVar3 = candidate_3cf9a(uVar2,bRam000585ef);
                        return uVar3;
                      }
                      bVar6 = *pbVar12;
                      if (bVar6 != 0x21) {
                        if ((bVar6 != 99) && (bVar6 != 0x43)) {
                          bVar7 = pbVar12[1];
                          psVar15 = (short *)0x3e194;
                          if (((((bVar7 == 0x61) || (psVar15 = (short *)0x3e1ce, bVar7 == 0x69)) ||
                               (psVar15 = (short *)0x3e1d8, bVar7 == 0x75)) ||
                              ((psVar15 = (short *)0x3e1f2, bVar7 == 0x65 ||
                               (psVar15 = (short *)0x3e214, bVar7 == 0x6f)))) ||
                             ((((psVar15 = (short *)0x3e23e, bVar7 == 0x41 ||
                                ((psVar15 = (short *)0x3e260, bVar7 == 0x49 ||
                                 (psVar15 = (short *)0x3e26a, bVar7 == 0x55)))) ||
                               (psVar15 = (short *)0x3e274, bVar7 == 0x45)) ||
                              (psVar15 = (short *)0x3e28e, bVar7 == 0x4f)))) {
                            while( true ) {
                              if (*psVar15 == 0) break;
                              if (*psVar15 == CONCAT11(bVar6,bVar7)) {
                                uVar3 = CONCAT22(sVar8,psVar15[1] | (uVar5 & 0xf00) << 4) - 0x10000U
                                        & 0xfffff000;
                                uVar5 = psVar15[2] | (ushort)uVar3;
                                sVar1 = (short)(uVar3 >> 0x10);
                                _DAT_003c0000 = CONCAT22(sVar1,uVar5);
                                return CONCAT22(uVar5,sVar1 + 0x21);
                              }
                              psVar15 = psVar15 + 4;
                            }
                          }
                          _DAT_003c0000 =
                               CONCAT31((int3)(CONCAT22(sVar8,(short)CONCAT31((int3)(CONCAT22(sVar8,
                                                  uVar5) >> 8),0x30) << 4) >> 8),bVar7);
                          return (uint)(ushort)(sVar8 + 0x20);
                        }
                        uVar3 = CONCAT31((int3)(CONCAT22(sVar8,(short)CONCAT31((int3)(CONCAT22(sVar8
                                                  ,uVar5) >> 8),0x30) << 4) >> 8),bVar6) + 0x10000U
                                & 0xffff0f00;
                        _DAT_003c0000 = uVar3 | 0xbeb;
                        return (uint)(ushort)((short)(uVar3 >> 0x10) + 0x1f);
                      }
                      uVar4 = 0xbea;
                      goto LAB_0003dfea;
                    }
                    if ((bVar7 < 0x80) || ((0x9f < bVar7 && (bVar7 < 0xe0)))) goto LAB_0003deb4;
                    puVar14 = (undefined4 *)0x3d512;
                    goto LAB_0003e02a;
                  }
                  iVar13 = 0x3d996;
                }
                if (bVar7 == 0x40) {
                  bVar7 = *pbVar12;
                  cVar10 = '\0';
                  if ((((bVar7 == 0x2e) || (cVar10 = '\x01', bVar7 == 0x3a)) ||
                      (cVar10 = '\x02', bVar7 == 0x7e)) || (bVar7 == 0x2d)) {
                    bVar7 = pbVar12[1];
                    cVar11 = cVar10;
                    if (((bVar7 == 0x61) || (cVar11 = cVar10 + '\x03', bVar7 == 0x75)) ||
                       ((cVar11 = cVar10 + '\x06', bVar7 == 0x65 ||
                        (cVar11 = cVar10 + '\b', bVar7 == 0x6f)))) {
                      bVar7 = cVar11 + 0x80;
                    }
                  }
                  else if (bVar7 == 0x60) {
                    bVar7 = pbVar12[1];
                  }
                }
                uVar4 = *(ushort *)(iVar13 + (short)((ushort)bVar7 * 2));
LAB_0003dfea:
                uVar4 = (uVar5 & 0xf00) << 4 | uVar4;
                _DAT_003c0000 = CONCAT22(sVar8,uVar4);
                return CONCAT22(uVar4,sVar8 + 0x20);
              }
LAB_0003deb4:
              uVar2 = CONCAT31((int3)(CONCAT22(sVar8,(short)CONCAT31((int3)(CONCAT22(sVar8,uVar5) >>
                                                                           8),0x80) << 4) >> 8),
                               (bVar7 & 0xf0) * '\x02');
              if (7 < (bVar7 & 0xf)) {
                uVar2 = CONCAT22(sVar8,(short)uVar2 + 0x100);
              }
              iVar13 = CONCAT31((int3)((uint)uVar2 >> 8),(bVar7 & 7) * '\x02' | (byte)uVar2);
              goto LAB_0003dede;
            }
            iVar13 = 0x3d796;
          }
          iVar13 = CONCAT22(sVar8,*(ushort *)(iVar13 + (short)((ushort)bVar7 * 2)) |
                                  (uVar5 & 0xf00) << 4);
          goto LAB_0003dede;
        }
        uVar17 = candidate_3dd96();
      }
      sVar1 = sVar1 + 1;
      if (3 < sVar1) {
        return (uint)(uVar17 >> 0x20);
      }
    } while( true );
  }
LAB_000582f2:
  *(undefined2 *)(unaff_A6 + 0x1e) = 0xffff;
  *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x3c;
  candidate_57ae0();
  for (sVar1 = 0; *(int *)(sVar1 + 0x585fc) != 0; sVar1 = sVar1 + 4) {
    bVar16 = false;
    candidate_57aa6();
    if (bVar16) {
      candidate_57a98();
    }
  }
  candidate_57ac4();
  uVar3 = (uint)(ushort)(*(short *)(unaff_A6 + 0x1c) + 0xa2);
  sVar1 = 8;
SUB_0003cf12:
  DAT_00300001 = (char)uVar3;
  for (; sVar1 != 0; sVar1 = sVar1 + -1) {
    uVar9 = CONCAT22((short)uVar3,0xff);
    for (sVar8 = 2; sVar8 != 0; sVar8 = sVar8 + -1) {
      if (0x74ffffff < uVar9) {
        __m68k_trap(0xf);
      }
      uVar9 = uVar9 + 0x10000;
    }
    uVar3 = (uint)(ushort)((short)uVar3 + 0x20);
  }
  _DAT_003c0004 = 0;
  _DAT_003c0000 = 0x82000000;
  return uVar3;
  while (puVar14 = puVar14 + 1, sVar1 != 0) {
LAB_0003e02a:
    uVar2 = *puVar14;
    sVar1 = (short)uVar2;
    if (CONCAT11(bVar7,*pbVar12) == sVar1) break;
  }
  iVar13 = CONCAT22(sVar8,(uVar5 & 0xf00) << 4 | (ushort)((uint)uVar2 >> 0x10));
LAB_0003dede:
  _DAT_003c0000 = iVar13 + 0x200001;
  return (uint)(ushort)((short)((uint)(iVar13 + 0x200001) >> 0x10) + 0x20);
}
