/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x092c9a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_92c9a(void)

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

code_r0x00092c9a:
  unaff_A6[0x24] = 0;
  if (*(byte *)((int)unaff_A6 + 0x77) < 7) {
    candidate_4256();
    *unaff_A6 = 0x92cc2;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
LAB_0009339e:
      candidate_4256();
      *unaff_A6 = 0x933c2;
      candidate_153d2();
      unaff_A6[0xd] = (-0x238e00 - unaff_A6[0xd] >> 6) + unaff_A6[0xd];
      candidate_ca2ce();
      uVar4 = *extraout_A0_07;
      iVar7 = 0;
      iVar6 = 6;
      if ((uVar4 & 4) != 0) {
        iVar7 = -0x2c000;
        iVar6 = 3;
      }
      if ((uVar4 & 8) != 0) {
        iVar7 = 0x2c000;
        iVar6 = 3;
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
        *unaff_A6 = candidate_9347a;
      }
LAB_00093692:
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
        *unaff_A6 = candidate_92ad8;
      }
      bVar9 = false;
      candidate_aa9e4();
      if ((bVar9) || ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
        if (*(char *)((int)unaff_A6 + 0x5a) == '\x0f') {
          *unaff_A6 = 0x93afa;
        }
        else {
          *unaff_A6 = candidate_9351a;
        }
      }
      if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) && (*(int *)((int)unaff_A6 + 0x62) != 0))
      {
        candidate_49e8();
      }
LAB_000939ba:
      *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)unaff_A6[0x1c];
      *(undefined1 *)((int)unaff_A6 + 0x87) = ((undefined1 *)unaff_A6[0x1c])[1];
      return;
    }
    candidate_ca2ce();
    uVar4 = *extraout_A0;
    bVar9 = false;
    if ((uVar4 & 8) != 0) goto LAB_00092d24;
    if ((uVar4 & 4) != 0) goto LAB_00092d74;
    if ((uVar4 & 0x80) == 0) {
      if ((uVar4 & 0x20) == 0) {
        if ((uVar4 & 2) != 0) goto LAB_00093138;
        if (((uVar4 & 0x40) != 0) && (candidate_92934(), bVar9)) {
LAB_00093054:
          do {
            *(undefined1 *)((int)unaff_A6 + 0x77) = 0;
            *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
            *(undefined1 *)((int)unaff_A6 + 0x79) = 0xff;
            candidate_4256();
            *unaff_A6 = 0x9307c;
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009339e;
            candidate_ca2ce();
            uVar4 = *extraout_A0_02;
            if (*(char *)((int)unaff_A6 + 0x79) != '\0') {
              if ((uVar4 & 8) == 0) {
                if ((uVar4 & 4) == 0) {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                }
                else {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffd6000;
                }
              }
              else {
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0x2a000;
              }
              goto LAB_000930ee;
            }
            if ((uVar4 & 2) == 0) goto code_r0x000930d6;
LAB_00093138:
            do {
              candidate_120f8();
              *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
              candidate_4256();
              *unaff_A6 = 0x9315e;
              if (*(char *)((int)unaff_A6 + 0x76) != *(char *)((int)unaff_A6 + 0x7a)) {
                *(char *)((int)unaff_A6 + 0x7a) = *(char *)((int)unaff_A6 + 0x76);
                candidate_4256();
              }
              if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009339e;
              candidate_ca2ce();
              uVar4 = *extraout_A0_03;
              if ((uVar4 & 8) == 0) {
                if ((uVar4 & 4) == 0) {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                }
                else {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffd6000;
                }
              }
              else {
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0x2a000;
              }
              uVar10 = 0;
              if ((uVar4 & 0x80) != 0) goto LAB_00093a08;
              if ((uVar4 & 0x20) != 0) goto LAB_000937c6;
              if ((uVar4 & 2) != 0) {
                if ((uVar4 & 0x40) != 0) {
                  candidate_929e6();
                }
                candidate_9380a();
                candidate_92832();
                if ((bool)uVar10) {
                  *(undefined2 *)((int)unaff_A6 + 0x8a) = 0;
                  *unaff_A6 = 0x93204;
                }
                goto LAB_00093692;
              }
LAB_00093278:
              candidate_120f8();
              *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
              candidate_4256();
              *unaff_A6 = 0x9329e;
              if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009339e;
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
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffd6000;
                }
              }
              else {
                *(undefined4 *)((int)unaff_A6 + 0x62) = 0x2a000;
              }
              uVar10 = 0;
              if ((uVar4 & 0x80) != 0) goto LAB_00093a08;
              if ((uVar4 & 0x20) != 0) goto LAB_0009333a;
            } while ((uVar4 & 2) != 0);
            if ((uVar4 & 0x40) == 0) {
              candidate_9380a();
              candidate_92832();
              if ((bool)uVar10) {
                *unaff_A6 = candidate_92c9a;
              }
              goto LAB_00093692;
            }
          } while( true );
        }
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        candidate_9380a();
        candidate_92832();
      }
      else {
LAB_0009333a:
        candidate_120f8();
        unaff_A6[0xd] = 0;
        candidate_4256();
        *unaff_A6 = 0x93362;
        candidate_ca2ce();
        if ((*extraout_A0_06 & 0x40) != 0) {
          candidate_92934();
        }
        uVar3 = candidate_14c0c();
        *(undefined2 *)(unaff_A6 + 0x21) = uVar3;
        candidate_92832();
        if (unaff_A6[0xd] != 0) {
          *unaff_A6 = 0x933c2;
        }
      }
      goto LAB_00093692;
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0x9321a;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009339e;
    candidate_ca2ce();
    uVar4 = *extraout_A0_04;
    if ((uVar4 & 8) != 0) goto LAB_00092d24;
    if ((uVar4 & 4) != 0) goto LAB_00092d74;
    if ((uVar4 & 2) == 0) goto LAB_00093278;
    if ((uVar4 & 0x80) == 0) {
      if ((uVar4 & 0x20) == 0) {
        if ((uVar4 & 0x40) != 0) {
          candidate_929e6();
        }
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        candidate_9380a();
        candidate_92832();
      }
      else {
LAB_000937c6:
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = candidate_937e4;
        uVar10 = 0;
        candidate_9380a();
        candidate_92832();
        if ((bool)uVar10) {
          candidate_aa7fc();
        }
        unaff_A6[0x14] = 0;
      }
      goto LAB_00093692;
    }
  }
LAB_00093a08:
  candidate_120f8();
  unaff_A6[0x24] = 0x93882;
  candidate_4256();
  candidate_aa7fc();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x93a46;
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
  candidate_93846();
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x8000;
  candidate_92832();
  if (((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) || (0x120 < *(short *)(unaff_A6 + 0xf)))
  goto LAB_00093ae6;
  uVar4 = candidate_49e8();
  bVar9 = uVar4 < 6;
  if (!bVar9) goto LAB_00093ad8;
  bVar9 = CARRY2(uVar4 * 2,uVar4 * 2);
  switch(uVar4) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    goto LAB_00093ae6;
  case 3:
    goto LAB_00093ae6;
  case 4:
    break;
  case 5:
  }
LAB_00093ad8:
  candidate_1945c();
  if (bVar9) {
LAB_00093ae6:
    candidate_49e8();
    *unaff_A6 = 0x93afa;
  }
  goto LAB_000939ba;
code_r0x000930d6:
  if ((uVar4 & 8) != 0) goto LAB_00092d52;
  if ((uVar4 & 4) != 0) {
LAB_00092da2:
    do {
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffd6000;
LAB_00092dc4:
      while( true ) {
        candidate_4256();
        *(undefined1 *)((int)unaff_A6 + 0x7a) = *(undefined1 *)((int)unaff_A6 + 0x76);
        *unaff_A6 = 0x92dda;
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
LAB_00092e42:
          bVar9 = *(byte *)((int)unaff_A6 + 0x77) < 5;
          if (bVar9) {
            if ((uVar4 & 0x80) != 0) goto LAB_00093a08;
            if ((uVar4 & 2) != 0) goto LAB_00093138;
            if ((uVar4 & 0x20) != 0) goto LAB_0009333a;
            if (((uVar4 & 0x40) != 0) && (candidate_92934(), bVar9)) goto LAB_00093054;
          }
          else {
            if ((uVar4 & 2) == 0) goto LAB_00093278;
            if ((uVar4 & 0x80) != 0) goto LAB_00093a08;
            if ((uVar4 & 0x20) != 0) goto LAB_000937c6;
            if ((uVar4 & 0x40) != 0) {
              candidate_929e6();
            }
          }
          candidate_9380a();
          bVar2 = *(byte *)((int)unaff_A6 + 0x7a);
          *(byte *)((int)unaff_A6 + 0x7a) = *(byte *)((int)unaff_A6 + 0x76);
          if ((char)(*(byte *)((int)unaff_A6 + 0x76) ^ bVar2) < '\0') {
            candidate_4256();
          }
          candidate_92832();
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            *unaff_A6 = 0x9339e;
          }
          cVar5 = *(char *)((int)unaff_A6 + 0x95);
          if ((cVar5 != '\0') &&
             (bVar2 = *(byte *)(unaff_A6 + 0x25), *(byte *)(unaff_A6 + 0x25) = bVar2 + cVar5 * -0x10
             , bVar2 < (byte)(cVar5 * '\x10'))) {
            *(char *)((int)unaff_A6 + 0x95) = *(char *)((int)unaff_A6 + 0x95) + -1;
            candidate_40bc();
          }
          goto LAB_00093692;
        }
        *(undefined1 *)((int)unaff_A6 + 0x95) = 3;
LAB_00092d52:
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0x2a000;
      }
      if ((uVar4 & 4) == 0) {
        *(char *)((int)unaff_A6 + 0x7a) = *(char *)((int)unaff_A6 + 0x76);
        if (*(char *)((int)unaff_A6 + 0x76) == '\0') goto code_r0x00092c9a;
        *(char *)((int)unaff_A6 + 0x7d) = -(*(int *)((int)unaff_A6 + 0x62) < 0);
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x92f94;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0009339e;
        candidate_ca2ce();
        uVar4 = *extraout_A0_01;
        if ((uVar4 & 8) == 0) {
          if ((uVar4 & 4) == 0) {
            bVar9 = *(byte *)((int)unaff_A6 + 0x77) < 5;
            if (bVar9) {
              if ((uVar4 & 0x80) != 0) goto LAB_00093a08;
              if ((uVar4 & 2) != 0) goto LAB_00093138;
              if ((uVar4 & 0x20) != 0) goto LAB_0009333a;
              if (((uVar4 & 0x40) != 0) && (candidate_92934(), bVar9)) goto LAB_00093054;
            }
            else {
              if ((uVar4 & 2) == 0) goto LAB_00093278;
              if ((uVar4 & 0x80) != 0) goto LAB_00093a08;
              if ((uVar4 & 0x20) != 0) goto LAB_000937c6;
              if ((uVar4 & 0x40) != 0) {
                candidate_929e6();
              }
            }
            bVar2 = *(byte *)((int)unaff_A6 + 0x7a);
            *(byte *)((int)unaff_A6 + 0x7a) = *(byte *)((int)unaff_A6 + 0x76);
            uVar10 = 0;
            if ((char)(*(byte *)((int)unaff_A6 + 0x76) ^ bVar2) < '\0') goto code_r0x00092c9a;
            candidate_9380a();
            candidate_92832();
            if ((bool)uVar10) {
              *unaff_A6 = candidate_92c9a;
            }
            goto LAB_00093692;
          }
          if (-1 < *(char *)((int)unaff_A6 + 0x7d)) goto LAB_00092d52;
LAB_00092d74:
          *(undefined1 *)((int)unaff_A6 + 0x95) = 5;
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffd6000;
        }
        else {
          if (*(char *)((int)unaff_A6 + 0x7d) < '\0') goto LAB_00092da2;
LAB_00092d24:
          *(undefined1 *)((int)unaff_A6 + 0x95) = 5;
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0x2a000;
        }
        goto LAB_00092dc4;
      }
      if (*(int *)((int)unaff_A6 + 0x62) < 0) goto LAB_00092e42;
      *(undefined1 *)((int)unaff_A6 + 0x95) = 3;
    } while( true );
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
LAB_000930ee:
  uVar10 = false;
  if ((uVar4 & 0x80) != 0) goto LAB_00093a08;
  if ((uVar4 & 0x20) != 0) goto LAB_0009333a;
  if (((uVar4 & 0x40) == 0) || (candidate_92934(), !(bool)uVar10)) {
    candidate_9380a();
    candidate_92832();
    if (((bool)uVar10) ||
       ((*(char *)((int)unaff_A6 + 0x7a) != '\0' && (*(char *)((int)unaff_A6 + 0x76) == '\0')))) {
      *unaff_A6 = candidate_92c9a;
    }
    goto LAB_00093692;
  }
  goto LAB_00093054;
}
