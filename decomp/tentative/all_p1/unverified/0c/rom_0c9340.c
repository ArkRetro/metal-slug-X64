/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c9340.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_c9340(void)

{
  undefined3 uVar1;
  uint uVar2;
  undefined1 uVar5;
  byte bVar6;
  short sVar4;
  char cVar7;
  undefined4 uVar3;
  char extraout_D1b;
  byte extraout_D1b_00;
  char unaff_D7b;
  ushort uVar8;
  int iVar9;
  undefined1 *puVar10;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar11;
  ulonglong uVar12;

  while( true ) {
    uVar2 = candidate_27c4();
    cVar7 = (char)uVar2;
    if (cVar7 == '\x01') goto LAB_000c9406;
    if (cVar7 == '\x02') goto LAB_000c9848;
    if (cVar7 != '\x03') break;
    while (cVar7 = candidate_27c4(), cVar7 != '\0') {
      if (cVar7 == '\x01') {
LAB_000c9406:
        candidate_5798c();
        uVar5 = 0;
        if (*(short *)((int)unaff_A6 + 0x6e) != 0) {
          uVar5 = 3;
        }
        *(undefined1 *)((int)unaff_A6 + 0x83) = uVar5;
        candidate_c9068();
        candidate_c9254();
        cVar7 = *(char *)(unaff_A5 + 0x7d88);
        *(char *)(unaff_A6 + 0x21) = cVar7;
        if (cVar7 == '\0') {
          *(undefined1 *)(unaff_A6 + 0x21) = 1;
        }
        if (DAT_0010c2e7 == '\x01') {
          *(undefined1 *)(unaff_A6 + 0x21) = 3;
        }
        if (DAT_0010c2e7 == '\x02') {
          *(undefined1 *)(unaff_A6 + 0x21) = 1;
        }
        *(undefined2 *)(unaff_A5 + 0x69da) = 0;
        *(undefined1 *)(unaff_A6 + 0x22) = 1;
        if ((thunk_FUN_0000447c[unaff_A5 + 3] == (code)0x0) ||
           (*(char *)((int)unaff_A6 + 0x82) < '\0')) {
          bVar11 = false;
          do {
            uVar2 = candidate_c9202();
            if (bVar11) break;
            uVar8 = (short)uVar2 + 1U & 3;
            uVar2 = (uint)uVar8;
            bVar6 = (byte)uVar8;
            bVar11 = bVar6 < *(byte *)((int)unaff_A6 + 0x83);
          } while (bVar6 != *(byte *)((int)unaff_A6 + 0x83));
          *(char *)((int)unaff_A6 + 0x83) = (char)uVar2;
          *(byte *)(unaff_A5 + 0x69e0) = *(byte *)(unaff_A5 + 0x69e0) | '\x01' << (uVar2 & 7);
          *(undefined1 *)((int)unaff_A6 + 0x82) = *(undefined1 *)((short)uVar2 + 0xc91fe);
          *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
          *unaff_A6 = 0xc94d6;
          while (candidate_c9216(), extraout_D1b != '\0') {
            candidate_c9254();
            candidate_40bc();
            *unaff_A6 = 0xc94fc;
            if (*(char *)(unaff_A5 + 0x69e3) == '\0') {
              if (*(char *)(unaff_A5 + 0x69dc) != '\0') {
                candidate_c907a();
                iVar9 = 0x10c35a;
                if (*(short *)((int)unaff_A6 + 0x6e) != 0) {
                  iVar9 = 0x10c360;
                }
                bVar6 = *(byte *)(iVar9 + 4);
                if ((bVar6 & 0x10) == 0) {
                  uVar3 = 0;
                  if ((bVar6 & 4) == 0) {
                    if ((bVar6 & 8) != 0) {
                      uVar3 = 1;
                    }
                  }
                  else {
                    uVar3 = 0xffffffff;
                  }
                  uVar12 = (ulonglong)CONCAT14(*(byte *)((int)unaff_A6 + 0x83),uVar3);
                  *(byte *)(unaff_A5 + 0x69e0) =
                       *(byte *)(unaff_A5 + 0x69e0) &
                       ~('\x01' << (*(byte *)((int)unaff_A6 + 0x83) & 7));
                  goto LAB_000c9554;
                }
              }
              break;
            }
            candidate_c9254();
            uVar2 = candidate_4192();
            *unaff_A6 = 0xc94cc;
            if (DAT_0010e9e3 != '\0') goto LAB_000c9b38;
          }
        }
        candidate_c90aa();
        *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
        candidate_c9254();
        *(undefined2 *)(unaff_A5 + 0x69da) = 0;
        *unaff_A6 = 0xc95ae;
        while( true ) {
          if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
            puVar10 = &DAT_0010c46a;
          }
          else {
            puVar10 = &DAT_0010c472;
          }
          puVar10[7] = *(undefined1 *)((int)unaff_A6 + 0x82);
          candidate_9aaa8();
          candidate_c9254();
          candidate_c9d08();
          if ((-1 < *(char *)(unaff_A6 + 0x21)) &&
             (cVar7 = *(char *)(unaff_A6 + 0x21), *(char *)(unaff_A6 + 0x21) = cVar7 + -1,
             cVar7 == '\0')) break;
          candidate_19124();
          candidate_41b0();
          bVar6 = *(byte *)((int)unaff_A6 + 0x82);
          *(byte *)(extraout_A0 + 0x5f) = *(byte *)(extraout_A0 + 0x5f) | 0x80;
          *(ushort *)(extraout_A0 + 0x6e) = (ushort)bVar6;
          *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)((int)unaff_A6 + 0x6e);
          *unaff_A6 = 0xc962c;
          uVar5 = 0;
          while (candidate_1964a(), !(bool)uVar5) {
            candidate_c9254();
            *unaff_A6 = 0xc9644;
            uVar5 = false;
            uVar2 = candidate_1964a();
            if (!(bool)uVar5) goto LAB_000c9b38;
          }
          uVar2 = func_0x000040ac();
          if ((bool)uVar5) {
            if (*(char *)(unaff_A5 + 0x69e1) != '\0') {
              if (*(char *)((int)unaff_A6 + 0x86) == '\0') {
                candidate_c9254();
                candidate_3d244();
                candidate_3d244();
                *(byte *)((int)unaff_A6 + 0x89) = *(byte *)((int)unaff_A6 + 0x89) | 4;
                *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
                *(undefined2 *)(unaff_A6 + 0x1f) = 0;
              }
              bVar6 = *(byte *)(unaff_A6 + 0x21);
              if ((char)bVar6 < '\0') {
                candidate_3d244();
              }
              else {
                candidate_3d0d0();
              }
              if (DAT_0010c2e7 == '\0') {
                candidate_3d004();
              }
              candidate_c071c();
              uVar8 = (ushort)bVar6;
              if (uVar8 == 0xffff) {
                candidate_3d244();
              }
              else {
                if (uVar8 == *(ushort *)((int)unaff_A6 + 0x7a)) {
                  *(byte *)((int)unaff_A6 + 0x89) = *(byte *)((int)unaff_A6 + 0x89) & 0xfe;
                }
                else {
                  *(ushort *)((int)unaff_A6 + 0x7a) = uVar8;
                  *(byte *)((int)unaff_A6 + 0x89) = *(byte *)((int)unaff_A6 + 0x89) ^ 1;
                }
                candidate_3d068();
              }
              cVar7 = candidate_c0764();
              if (extraout_D1b_00 < 4) {
                bVar6 = *(byte *)((int)unaff_A6 + 0x89);
                *(byte *)((int)unaff_A6 + 0x89) = bVar6 | 4;
                if ((bVar6 & 4) == 0) {
                  candidate_3d244();
                }
              }
              else {
                bVar6 = *(byte *)((int)unaff_A6 + 0x89);
                *(byte *)((int)unaff_A6 + 0x89) = bVar6 & 0xfb;
                if ((bVar6 & 4) != 0) {
                  candidate_3d244();
                }
              }
              if (cVar7 == *(char *)((int)unaff_A6 + 0x8a)) {
                *(byte *)((int)unaff_A6 + 0x89) = *(byte *)((int)unaff_A6 + 0x89) & 0xfe;
              }
              else {
                *(char *)((int)unaff_A6 + 0x8a) = cVar7;
                *(byte *)((int)unaff_A6 + 0x89) = *(byte *)((int)unaff_A6 + 0x89) ^ 1;
              }
              candidate_3d068();
              candidate_c9b94();
              uVar2 = candidate_c9d08();
            }
            *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(unaff_A5 + 0x69e1);
            goto LAB_000c9b30;
          }
        }
        *(byte *)(unaff_A5 + 0x69e0) =
             *(byte *)(unaff_A5 + 0x69e0) & ~('\x01' << (*(byte *)((int)unaff_A6 + 0x83) & 7));
      }
      else if (cVar7 != '\x02') {
        uVar3 = candidate_c9254();
        if ((DAT_0010c2e7 == '\0') && (uVar3 = candidate_57b44(), -1 < (short)uVar3)) {
          *(undefined1 *)(unaff_A6 + 0x18) = 0;
          bVar11 = false;
          candidate_40bc();
          if (bVar11) {
            sVar4 = *(short *)((int)unaff_A6 + 0x6e);
            *(short *)(extraout_A0_00 + 0x6e) = sVar4;
            if (sVar4 == 0) {
              uVar3 = 0x10c46e;
            }
            else {
              uVar3 = 0x10c476;
            }
            uVar2 = (uint)(ushort)(*(short *)(unaff_A6 + 0x1d) + 0x40U);
            *(ushort *)(extraout_A0_00 + 0x70) = *(short *)(unaff_A6 + 0x1d) + 0x40U;
            *(undefined4 *)(extraout_A0_00 + 0x72) = uVar3;
            *unaff_A6 = 0xc9a8c;
            if (*(char *)(unaff_A6 + 0x18) == '\0') goto LAB_000c9b30;
            candidate_c9254();
            candidate_4192();
          }
          uVar3 = candidate_57c82();
        }
        *(undefined1 *)(unaff_A6 + 0x22) = 3;
        *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
        *unaff_A6 = 0xc9ace;
        uVar1 = (undefined3)((uint)uVar3 >> 8);
        if (*(char *)(unaff_A6 + 0x20) == '\0') {
          bVar6 = -(*(char *)(unaff_A5 + 0x7db7) == '\x01') |
                  -(*(char *)(unaff_A5 + 0x7db6) == '\x01');
          uVar2 = CONCAT31(uVar1,bVar6);
          if (bVar6 != 0) break;
        }
        else {
          uVar8 = (short)CONCAT31(uVar1,*(char *)(unaff_A6 + 0x20)) - 1;
          uVar2 = (uint)uVar8;
          *(ushort *)(unaff_A6 + 0x20) = uVar8;
        }
        cVar7 = *(char *)(unaff_A5 + 0x69e1);
        *(char *)((int)unaff_A6 + 0x86) = cVar7;
        if (cVar7 != '\0') {
          uVar2 = candidate_3d25a();
        }
        *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(unaff_A5 + 0x69e1);
        goto LAB_000c9b30;
      }
LAB_000c9848:
      if ((DAT_0010fd8a != '\0') && (DAT_0010c2e7 == '\0')) {
        uVar5 = candidate_1a872();
        *(undefined1 *)(unaff_A6 + 0x23) = uVar5;
        bVar11 = *(short *)((int)unaff_A6 + 0x6e) == 0;
        candidate_1a8aa();
        if ((bVar11) && (*(char *)(unaff_A5 + 0x7d82) == '\0')) {
          candidate_1a900();
          if (unaff_D7b == '\0') {
            candidate_1901a();
          }
          else {
            candidate_19008();
          }
        }
        else {
          candidate_19798();
          candidate_c9254();
          candidate_19008();
          *unaff_A6 = candidate_c98cc;
          cVar7 = candidate_27c4();
          if (cVar7 == '\0') break;
          if (cVar7 != '\x03') {
            *(char *)(unaff_A6 + 0x22) = cVar7;
            bVar6 = candidate_1a872();
            if (*(byte *)(unaff_A6 + 0x23) < bVar6) {
              *(byte *)(unaff_A6 + 0x23) = bVar6;
              candidate_19008();
            }
            sVar4 = candidate_19022();
            if (sVar4 != 0) {
              uVar2 = (uint)*(ushort *)((int)unaff_A6 + 0x6e);
              if ((*(byte *)(unaff_A5 + 0x7003) & '\x01' << (*(ushort *)((int)unaff_A6 + 0x6e) & 7))
                  == 0) goto LAB_000c9b38;
              if (*(char *)(unaff_A5 + 0x69e1) != '\0') {
                if (*(char *)((int)unaff_A6 + 0x86) == '\0') {
                  candidate_3d25a();
                }
                candidate_19022();
                uVar2 = candidate_3cf60();
              }
              *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(unaff_A5 + 0x69e1);
              goto LAB_000c9b30;
            }
          }
        }
      }
    }
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
    candidate_c8c96();
    candidate_5798c();
    *(undefined1 *)(unaff_A6 + 0x22) = 0;
    candidate_c9254();
    *(undefined2 *)(unaff_A5 + 0x69da) = 0;
    *unaff_A6 = candidate_c9340;
  }
  if (*(char *)(unaff_A5 + 0x69e1) != '\0') {
    uVar2 = (uint)*(ushort *)((int)unaff_A6 + 0x6e);
    if ((*(byte *)(unaff_A5 + 0x7003) & '\x01' << (*(ushort *)((int)unaff_A6 + 0x6e) & 7)) == 0) {
LAB_000c9b38:
      if (DAT_0010c2e7 == '\0') {
        *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
        cVar7 = *(char *)(unaff_A5 + 0x69e1);
        *(char *)((int)unaff_A6 + 0x86) = cVar7;
        if (cVar7 != '\0') {
          uVar2 = candidate_c9228();
          if ((short)uVar2 != 0) {
            if ((short)uVar2 < 0) {
              uVar2 = candidate_c927a();
            }
            else {
              uVar2 = candidate_3d25a();
            }
          }
        }
      }
      return uVar2;
    }
    bVar11 = *(short *)((int)unaff_A6 + 0x6e) == 0;
    candidate_1a8aa();
    cVar7 = *(char *)((int)unaff_A6 + 0x85);
    *(char *)((int)unaff_A6 + 0x85) = -!bVar11;
    if (cVar7 != -!bVar11) {
      *(undefined2 *)(unaff_A5 + 0x69da) = 0;
    }
    uVar2 = candidate_c9228();
    if ((short)uVar2 != 0) {
      if ((short)uVar2 < 0) {
        uVar2 = candidate_c927a();
      }
      else {
        uVar2 = candidate_3d25a();
      }
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(unaff_A5 + 0x69e1);
  goto LAB_000c9b30;
  while( true ) {
    uVar12 = candidate_c9202();
    uVar2 = (uint)(uVar12 >> 0x20);
    if (bVar11) break;
LAB_000c9554:
    uVar2 = (byte)((char)uVar12 + (char)(uVar12 >> 0x20)) & 0xffff0003;
    bVar11 = (byte)uVar2 < *(byte *)((int)unaff_A6 + 0x83);
    if ((byte)uVar2 == *(byte *)((int)unaff_A6 + 0x83)) break;
  }
  *(char *)((int)unaff_A6 + 0x83) = (char)uVar2;
  *(byte *)(unaff_A5 + 0x69e0) = *(byte *)(unaff_A5 + 0x69e0) | '\x01' << (uVar2 & 7);
  uVar2 = uVar2 & 0xffffff03;
  *(undefined1 *)((int)unaff_A6 + 0x82) = *(undefined1 *)((short)uVar2 + 0xc91fe);
  *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(unaff_A5 + 0x69e1);
LAB_000c9b30:
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  return uVar2;
}
