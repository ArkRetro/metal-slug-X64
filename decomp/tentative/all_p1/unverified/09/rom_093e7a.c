/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x093e7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_93e7a(void)

{
  short *psVar1;
  byte bVar2;
  undefined2 uVar3;
  char cVar5;
  ushort uVar4;
  undefined1 extraout_D1b;
  int iVar6;
  int iVar7;
  ushort *extraout_A0;
  ushort *extraout_A0_00;
  ushort *extraout_A0_01;
  ushort *extraout_A0_02;
  ushort *extraout_A0_03;
  ushort *extraout_A0_04;
  ushort *extraout_A0_05;
  ushort *extraout_A0_06;
  ushort *extraout_A0_07;
  short *psVar8;
  undefined4 *unaff_A6;
  bool bVar9;
  undefined1 uVar10;

code_r0x00093e7a:
  unaff_A6[0x24] = 0;
  if (*(byte *)((int)unaff_A6 + 0x77) < 7) {
    candidate_4256();
    *unaff_A6 = 0x93ea2;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
LAB_0009457e:
      candidate_4256();
      *unaff_A6 = 0x945a2;
      candidate_153d2();
      unaff_A6[0xd] = (-0x5c28c0 - unaff_A6[0xd] >> 6) + unaff_A6[0xd];
      candidate_ca2ce();
      uVar4 = *extraout_A0_07;
      iVar7 = 0;
      iVar6 = 6;
      if ((uVar4 & 4) != 0) {
        iVar7 = -0x40000;
        iVar6 = 2;
      }
      if ((uVar4 & 8) != 0) {
        iVar7 = 0x40000;
        iVar6 = 2;
      }
      unaff_A6[0xc] = (iVar7 - unaff_A6[0xc] >> iVar6) + unaff_A6[0xc];
      if ((uVar4 & 0x40) != 0) {
        candidate_92934();
      }
      uVar3 = candidate_14c0c();
      *(undefined2 *)(unaff_A6 + 0x21) = uVar3;
      bVar9 = false;
      candidate_92832();
      if (bVar9) {
        candidate_4256();
      }
      if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) && ((int)unaff_A6[0xd] < 0)) {
        *unaff_A6 = candidate_9465a;
      }
LAB_00094872:
      if (*(short *)((int)unaff_A6 + 0x8a) != 0) {
        *(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1;
      }
      bVar2 = *(byte *)((int)unaff_A6 + 0x5f);
      *(byte *)((int)unaff_A6 + 0x5f) = bVar2 & 0xbf;
      if (((bVar2 & 0x40) != 0) && (*(char *)((int)unaff_A6 + 0x5a) != '\x0f')) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      }
      psVar1 = (short *)unaff_A6[0x24];
      if (psVar1 == (short *)0x0) {
        *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
        if (*(char *)((int)unaff_A6 + 0x7e) != '\0') {
          bVar2 = *(char *)((int)unaff_A6 + 0x7e) - 1;
          *(byte *)((int)unaff_A6 + 0x7e) = bVar2;
          if ((bVar2 & 1) != 0) {
            *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
            *(undefined1 *)(unaff_A6 + 0x22) = 0xff;
          }
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
        }
      }
      else {
        *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
        psVar8 = psVar1 + 1;
        if (*psVar1 == -1) {
          psVar8 = (short *)((int)psVar8 + *psVar8 + 2);
        }
        unaff_A6[0x24] = psVar8;
        candidate_9d1e();
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      }
      if ((DAT_0010f000 & 0x80) != 0) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      }
      bVar9 = false;
      candidate_aa760();
      if (bVar9) {
        cVar5 = candidate_aaa88();
        if (cVar5 == '\x01') {
          *(undefined1 *)((int)unaff_A6 + 0x7b) = extraout_D1b;
          *(undefined1 *)(unaff_A6 + 0x1f) = 0;
        }
      }
      else {
        *unaff_A6 = candidate_93d40;
      }
      bVar9 = false;
      candidate_aa9e4();
      if ((bVar9) || ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
        if (*(char *)((int)unaff_A6 + 0x5a) == '\x0f') {
          *unaff_A6 = 0x94cda;
        }
        else {
          *unaff_A6 = candidate_946fa;
        }
      }
      if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) && (*(int *)((int)unaff_A6 + 0x62) != 0))
      {
        candidate_49e8();
      }
LAB_00094b9a:
      *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)unaff_A6[0x1c];
      *(undefined1 *)((int)unaff_A6 + 0x87) = ((undefined1 *)unaff_A6[0x1c])[1];
      return;
    }
    candidate_ca2ce();
    uVar4 = *extraout_A0;
    bVar9 = false;
    if ((uVar4 & 8) != 0) goto LAB_00093f04;
    if ((uVar4 & 4) != 0) goto LAB_00093f54;
    if ((uVar4 & 0x80) == 0) {
      if ((uVar4 & 0x20) == 0) {
        if ((uVar4 & 2) != 0) goto LAB_00094318;
        if (((uVar4 & 0x40) != 0) && (candidate_92934(), bVar9)) {
LAB_00094234:
          do {
            *(undefined1 *)((int)unaff_A6 + 0x77) = 0;
            *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
            *(undefined1 *)((int)unaff_A6 + 0x79) = 0xff;
            candidate_4256();
            *unaff_A6 = 0x9425c;
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009457e;
            candidate_ca2ce();
            uVar4 = *extraout_A0_02;
            if (*(char *)((int)unaff_A6 + 0x79) != '\0') {
              if ((uVar4 & 8) == 0) {
                if ((uVar4 & 4) == 0) {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                }
                else {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffc0000;
                }
              }
              else {
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0x40000;
              }
              goto LAB_000942ce;
            }
            if ((uVar4 & 2) == 0) goto code_r0x000942b6;
LAB_00094318:
            do {
              candidate_120f8();
              *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
              candidate_4256();
              *unaff_A6 = 0x9433e;
              if (*(char *)((int)unaff_A6 + 0x76) != *(char *)((int)unaff_A6 + 0x7a)) {
                *(char *)((int)unaff_A6 + 0x7a) = *(char *)((int)unaff_A6 + 0x76);
                candidate_4256();
              }
              if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009457e;
              candidate_ca2ce();
              uVar4 = *extraout_A0_03;
              if ((uVar4 & 8) == 0) {
                if ((uVar4 & 4) == 0) {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                }
                else {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffc0000;
                }
              }
              else {
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0x40000;
              }
              uVar10 = 0;
              if ((uVar4 & 0x80) != 0) goto LAB_00094be8;
              if ((uVar4 & 0x20) != 0) goto LAB_000949a6;
              if ((uVar4 & 2) != 0) {
                if ((uVar4 & 0x40) != 0) {
                  candidate_929e6();
                }
                candidate_949ea();
                candidate_92832();
                if ((bool)uVar10) {
                  *(undefined2 *)((int)unaff_A6 + 0x8a) = 0;
                  *unaff_A6 = 0x943e4;
                }
                goto LAB_00094872;
              }
LAB_00094458:
              candidate_120f8();
              *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
              candidate_4256();
              *unaff_A6 = 0x9447e;
              if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009457e;
              if (*(char *)((int)unaff_A6 + 0x76) != *(char *)((int)unaff_A6 + 0x7a)) {
                *(char *)((int)unaff_A6 + 0x7a) = *(char *)((int)unaff_A6 + 0x76);
                candidate_4256();
              }
              candidate_ca2ce();
              uVar4 = *extraout_A0_05;
              if ((uVar4 & 8) == 0) {
                if ((uVar4 & 4) == 0) {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                }
                else {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffc0000;
                }
              }
              else {
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0x40000;
              }
              uVar10 = 0;
              if ((uVar4 & 0x80) != 0) goto LAB_00094be8;
              if ((uVar4 & 0x20) != 0) goto LAB_0009451a;
            } while ((uVar4 & 2) != 0);
            if ((uVar4 & 0x40) == 0) {
              candidate_949ea();
              candidate_92832();
              if ((bool)uVar10) {
                *unaff_A6 = candidate_93e7a;
              }
              goto LAB_00094872;
            }
          } while( true );
        }
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        candidate_949ea();
        candidate_92832();
      }
      else {
LAB_0009451a:
        candidate_120f8();
        unaff_A6[0xd] = 0;
        candidate_4256();
        *unaff_A6 = 0x94542;
        candidate_ca2ce();
        if ((*extraout_A0_06 & 0x40) != 0) {
          candidate_92934();
        }
        uVar3 = candidate_14c0c();
        *(undefined2 *)(unaff_A6 + 0x21) = uVar3;
        candidate_92832();
        if (unaff_A6[0xd] != 0) {
          *unaff_A6 = 0x945a2;
        }
      }
      goto LAB_00094872;
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0x943fa;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009457e;
    candidate_ca2ce();
    uVar4 = *extraout_A0_04;
    if ((uVar4 & 8) != 0) goto LAB_00093f04;
    if ((uVar4 & 4) != 0) goto LAB_00093f54;
    if ((uVar4 & 2) == 0) goto LAB_00094458;
    if ((uVar4 & 0x80) == 0) {
      if ((uVar4 & 0x20) == 0) {
        if ((uVar4 & 0x40) != 0) {
          candidate_929e6();
        }
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        candidate_949ea();
        candidate_92832();
      }
      else {
LAB_000949a6:
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = candidate_949c4;
        uVar10 = 0;
        candidate_949ea();
        candidate_92832();
        if ((bool)uVar10) {
          candidate_aa7fc();
        }
        unaff_A6[0x14] = 0;
      }
      goto LAB_00094872;
    }
  }
LAB_00094be8:
  candidate_120f8();
  unaff_A6[0x24] = 0x94a62;
  candidate_4256();
  candidate_aa7fc();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x94c26;
  psVar1 = (short *)unaff_A6[0x24];
  if (psVar1 != (short *)0x0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    psVar8 = psVar1 + 1;
    if (*psVar1 == -1) {
      psVar8 = (short *)((int)psVar8 + *psVar8 + 2);
    }
    unaff_A6[0x24] = psVar8;
    candidate_9d1e();
  }
  *(int *)((int)unaff_A6 + 0x62) =
       (0xc0000 - *(int *)((int)unaff_A6 + 0x62) >> 5) + *(int *)((int)unaff_A6 + 0x62);
  candidate_94a26();
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x8000;
  candidate_92832();
  if (((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) || (0x120 < *(short *)(unaff_A6 + 0xf)))
  goto LAB_00094cc6;
  uVar4 = candidate_49e8();
  bVar9 = uVar4 < 6;
  if (!bVar9) goto LAB_00094cb8;
  bVar9 = CARRY2(uVar4 * 2,uVar4 * 2);
  switch(uVar4) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    goto LAB_00094cc6;
  case 3:
    goto LAB_00094cc6;
  case 4:
    break;
  case 5:
  }
LAB_00094cb8:
  candidate_1945c();
  if (bVar9) {
LAB_00094cc6:
    candidate_49e8();
    *unaff_A6 = 0x94cda;
  }
  goto LAB_00094b9a;
code_r0x000942b6:
  if ((uVar4 & 8) != 0) goto LAB_00093f32;
  if ((uVar4 & 4) != 0) {
LAB_00093f82:
    do {
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffc0000;
LAB_00093fa4:
      while( true ) {
        candidate_4256();
        *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
        *unaff_A6 = 0x93fba;
        candidate_ca2ce();
        uVar4 = *extraout_A0_00;
        if ((uVar4 & 1) == 0) {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xdf;
        }
        else {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x20;
        }
        if ((uVar4 & 2) == 0) {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xef;
        }
        else {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x10;
        }
        if ((uVar4 & 8) == 0) break;
        if (0 < *(int *)((int)unaff_A6 + 0x62)) {
LAB_00094022:
          bVar9 = *(byte *)((int)unaff_A6 + 0x77) < 5;
          if (bVar9) {
            if ((uVar4 & 0x80) != 0) goto LAB_00094be8;
            if ((uVar4 & 2) != 0) goto LAB_00094318;
            if ((uVar4 & 0x20) != 0) goto LAB_0009451a;
            if (((uVar4 & 0x40) != 0) && (candidate_92934(), bVar9)) goto LAB_00094234;
          }
          else {
            if ((uVar4 & 2) == 0) goto LAB_00094458;
            if ((uVar4 & 0x80) != 0) goto LAB_00094be8;
            if ((uVar4 & 0x20) != 0) goto LAB_000949a6;
            if ((uVar4 & 0x40) != 0) {
              candidate_929e6();
            }
          }
          candidate_949ea();
          bVar2 = *(byte *)((int)unaff_A6 + 0x7a);
          *(byte *)((int)unaff_A6 + 0x7a) = *(byte *)((int)unaff_A6 + 0x76);
          if ((char)(*(byte *)((int)unaff_A6 + 0x76) ^ bVar2) < '\0') {
            candidate_4256();
          }
          candidate_92832();
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            *unaff_A6 = 0x9457e;
          }
          cVar5 = *(char *)((int)unaff_A6 + 0x95);
          if ((cVar5 != '\0') &&
             (bVar2 = *(byte *)(unaff_A6 + 0x25), *(byte *)(unaff_A6 + 0x25) = bVar2 + cVar5 * -0x10
             , bVar2 < (byte)(cVar5 * '\x10'))) {
            *(char *)((int)unaff_A6 + 0x95) = *(char *)((int)unaff_A6 + 0x95) + -1;
            candidate_40bc();
          }
          goto LAB_00094872;
        }
        *(undefined1 *)((int)unaff_A6 + 0x95) = 3;
LAB_00093f32:
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0x40000;
      }
      if ((uVar4 & 4) == 0) {
        *(char *)((int)unaff_A6 + 0x7a) = *(char *)((int)unaff_A6 + 0x76);
        if (*(char *)((int)unaff_A6 + 0x76) == '\0') goto code_r0x00093e7a;
        *(char *)((int)unaff_A6 + 0x7d) = -(*(int *)((int)unaff_A6 + 0x62) < 0);
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x94174;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009457e;
        candidate_ca2ce();
        uVar4 = *extraout_A0_01;
        if ((uVar4 & 8) == 0) {
          if ((uVar4 & 4) == 0) {
            bVar9 = *(byte *)((int)unaff_A6 + 0x77) < 5;
            if (bVar9) {
              if ((uVar4 & 0x80) != 0) goto LAB_00094be8;
              if ((uVar4 & 2) != 0) goto LAB_00094318;
              if ((uVar4 & 0x20) != 0) goto LAB_0009451a;
              if (((uVar4 & 0x40) != 0) && (candidate_92934(), bVar9)) goto LAB_00094234;
            }
            else {
              if ((uVar4 & 2) == 0) goto LAB_00094458;
              if ((uVar4 & 0x80) != 0) goto LAB_00094be8;
              if ((uVar4 & 0x20) != 0) goto LAB_000949a6;
              if ((uVar4 & 0x40) != 0) {
                candidate_929e6();
              }
            }
            bVar2 = *(byte *)((int)unaff_A6 + 0x7a);
            *(byte *)((int)unaff_A6 + 0x7a) = *(byte *)((int)unaff_A6 + 0x76);
            uVar10 = 0;
            if ((char)(*(byte *)((int)unaff_A6 + 0x76) ^ bVar2) < '\0') goto code_r0x00093e7a;
            candidate_949ea();
            candidate_92832();
            if ((bool)uVar10) {
              *unaff_A6 = candidate_93e7a;
            }
            goto LAB_00094872;
          }
          if (-1 < *(char *)((int)unaff_A6 + 0x7d)) goto LAB_00093f32;
LAB_00093f54:
          *(undefined1 *)((int)unaff_A6 + 0x95) = 5;
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffc0000;
        }
        else {
          if (*(char *)((int)unaff_A6 + 0x7d) < '\0') goto LAB_00093f82;
LAB_00093f04:
          *(undefined1 *)((int)unaff_A6 + 0x95) = 5;
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0x40000;
        }
        goto LAB_00093fa4;
      }
      if (*(int *)((int)unaff_A6 + 0x62) < 0) goto LAB_00094022;
      *(undefined1 *)((int)unaff_A6 + 0x95) = 3;
    } while( true );
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
LAB_000942ce:
  uVar10 = false;
  if ((uVar4 & 0x80) != 0) goto LAB_00094be8;
  if ((uVar4 & 0x20) != 0) goto LAB_0009451a;
  if (((uVar4 & 0x40) == 0) || (candidate_92934(), !(bool)uVar10)) {
    candidate_949ea();
    candidate_92832();
    if (((bool)uVar10) ||
       ((*(char *)((int)unaff_A6 + 0x7a) != '\0' && (*(char *)((int)unaff_A6 + 0x76) == '\0')))) {
      *unaff_A6 = candidate_93e7a;
    }
    goto LAB_00094872;
  }
  goto LAB_00094234;
}
