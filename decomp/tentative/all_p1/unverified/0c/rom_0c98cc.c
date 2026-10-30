/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c98cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_c98cc(void)

{
  undefined3 uVar1;
  uint uVar2;
  undefined1 uVar4;
  byte bVar5;
  short sVar3;
  char cVar6;
  char extraout_D1b;
  undefined4 uVar7;
  byte extraout_D1b_00;
  ushort uVar8;
  char unaff_D7b;
  int iVar9;
  undefined1 *puVar10;
  int extraout_A0;
  code *pcVar11;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar12;
  ulonglong uVar13;

  if (*(char *)((int)*(short *)((int)unaff_A6 + 0x6e) + unaff_A5 + 0x7db6) != '\x01')
  goto LAB_000c9918;
  pcVar11 = (code *)(unaff_A5 + 0x446d);
  if (*(short *)((int)unaff_A6 + 0x6e) != 0) {
    pcVar11 = FUN_00004470 + unaff_A5 + 5;
  }
  bVar5 = ((byte)*pcVar11 & 0xf) + 1;
  if (9 < bVar5) {
    bVar5 = 9;
  }
  *pcVar11 = (code)(bVar5 | (byte)*pcVar11 & 0xf0);
  candidate_c8cb2();
LAB_000c9420:
  candidate_c9068();
  candidate_c9254();
  cVar6 = *(char *)(unaff_A5 + 0x7d88);
  *(char *)(unaff_A6 + 0x21) = cVar6;
  if (cVar6 == '\0') {
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
  if ((thunk_FUN_0000447c[unaff_A5 + 3] == (code)0x0) || (*(char *)((int)unaff_A6 + 0x82) < '\0')) {
    bVar12 = false;
    do {
      uVar2 = candidate_c9202();
      if (bVar12) break;
      uVar8 = (short)uVar2 + 1U & 3;
      uVar2 = (uint)uVar8;
      bVar5 = (byte)uVar8;
      bVar12 = bVar5 < *(byte *)((int)unaff_A6 + 0x83);
    } while (bVar5 != *(byte *)((int)unaff_A6 + 0x83));
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
          bVar5 = *(byte *)(iVar9 + 4);
          if ((bVar5 & 0x10) == 0) {
            uVar7 = 0;
            if ((bVar5 & 4) == 0) {
              if ((bVar5 & 8) != 0) {
                uVar7 = 1;
              }
            }
            else {
              uVar7 = 0xffffffff;
            }
            uVar13 = (ulonglong)CONCAT14(*(byte *)((int)unaff_A6 + 0x83),uVar7);
            *(byte *)(unaff_A5 + 0x69e0) =
                 *(byte *)(unaff_A5 + 0x69e0) & ~('\x01' << (*(byte *)((int)unaff_A6 + 0x83) & 7));
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
       (cVar6 = *(char *)(unaff_A6 + 0x21), *(char *)(unaff_A6 + 0x21) = cVar6 + -1, cVar6 == '\0'))
    break;
    candidate_19124();
    candidate_41b0();
    bVar5 = *(byte *)((int)unaff_A6 + 0x82);
    *(byte *)(extraout_A0 + 0x5f) = *(byte *)(extraout_A0 + 0x5f) | 0x80;
    *(ushort *)(extraout_A0 + 0x6e) = (ushort)bVar5;
    *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)((int)unaff_A6 + 0x6e);
    *unaff_A6 = 0xc962c;
    uVar4 = 0;
    while (candidate_1964a(), !(bool)uVar4) {
      candidate_c9254();
      *unaff_A6 = 0xc9644;
      uVar4 = false;
      uVar2 = candidate_1964a();
      if (!(bool)uVar4) goto LAB_000c9b38;
    }
    uVar2 = func_0x000040ac();
    if ((bool)uVar4) {
      if (*(char *)(unaff_A5 + 0x69e1) != '\0') {
        if (*(char *)((int)unaff_A6 + 0x86) == '\0') {
          candidate_c9254();
          candidate_3d244();
          candidate_3d244();
          *(byte *)((int)unaff_A6 + 0x89) = *(byte *)((int)unaff_A6 + 0x89) | 4;
          *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
          *(undefined2 *)(unaff_A6 + 0x1f) = 0;
        }
        bVar5 = *(byte *)(unaff_A6 + 0x21);
        if ((char)bVar5 < '\0') {
          candidate_3d244();
        }
        else {
          candidate_3d0d0();
        }
        if (DAT_0010c2e7 == '\0') {
          candidate_3d004();
        }
        candidate_c071c();
        uVar8 = (ushort)bVar5;
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
        cVar6 = candidate_c0764();
        if (extraout_D1b_00 < 4) {
          bVar5 = *(byte *)((int)unaff_A6 + 0x89);
          *(byte *)((int)unaff_A6 + 0x89) = bVar5 | 4;
          if ((bVar5 & 4) == 0) {
            candidate_3d244();
          }
        }
        else {
          bVar5 = *(byte *)((int)unaff_A6 + 0x89);
          *(byte *)((int)unaff_A6 + 0x89) = bVar5 & 0xfb;
          if ((bVar5 & 4) != 0) {
            candidate_3d244();
          }
        }
        if (cVar6 == *(char *)((int)unaff_A6 + 0x8a)) {
          *(byte *)((int)unaff_A6 + 0x89) = *(byte *)((int)unaff_A6 + 0x89) & 0xfe;
        }
        else {
          *(char *)((int)unaff_A6 + 0x8a) = cVar6;
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
LAB_000c9848:
  do {
    if ((DAT_0010fd8a != '\0') && (DAT_0010c2e7 == '\0')) {
      uVar4 = candidate_1a872();
      *(undefined1 *)(unaff_A6 + 0x23) = uVar4;
      bVar12 = *(short *)((int)unaff_A6 + 0x6e) == 0;
      candidate_1a8aa();
      if ((bVar12) && (*(char *)(unaff_A5 + 0x7d82) == '\0')) {
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
LAB_000c9918:
        cVar6 = candidate_27c4();
        if (cVar6 == '\0') goto LAB_000c9312;
        if (cVar6 != '\x03') {
          *(char *)(unaff_A6 + 0x22) = cVar6;
          bVar5 = candidate_1a872();
          if (*(byte *)(unaff_A6 + 0x23) < bVar5) {
            *(byte *)(unaff_A6 + 0x23) = bVar5;
            candidate_19008();
          }
          sVar3 = candidate_19022();
          if (sVar3 != 0) {
            uVar2 = (uint)*(ushort *)((int)unaff_A6 + 0x6e);
            if ((*(byte *)(unaff_A5 + 0x7003) & '\x01' << (*(ushort *)((int)unaff_A6 + 0x6e) & 7))
                == 0) {
LAB_000c9b38:
              if (DAT_0010c2e7 == '\0') {
                *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
                cVar6 = *(char *)(unaff_A5 + 0x69e1);
                *(char *)((int)unaff_A6 + 0x86) = cVar6;
                if (cVar6 != '\0') {
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
    while (cVar6 = candidate_27c4(), cVar6 == '\0') {
LAB_000c9312:
      *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
      candidate_c8c96();
      candidate_5798c();
      *(undefined1 *)(unaff_A6 + 0x22) = 0;
      candidate_c9254();
      *(undefined2 *)(unaff_A5 + 0x69da) = 0;
      *unaff_A6 = candidate_c9340;
      uVar2 = candidate_27c4();
      cVar6 = (char)uVar2;
      if (cVar6 == '\x01') goto LAB_000c9406;
      if (cVar6 == '\x02') goto LAB_000c9848;
      if (cVar6 != '\x03') {
        if (*(char *)(unaff_A5 + 0x69e1) != '\0') {
          uVar2 = (uint)*(ushort *)((int)unaff_A6 + 0x6e);
          if ((*(byte *)(unaff_A5 + 0x7003) & '\x01' << (*(ushort *)((int)unaff_A6 + 0x6e) & 7)) ==
              0) goto LAB_000c9b38;
          bVar12 = *(short *)((int)unaff_A6 + 0x6e) == 0;
          candidate_1a8aa();
          cVar6 = *(char *)((int)unaff_A6 + 0x85);
          *(char *)((int)unaff_A6 + 0x85) = -!bVar12;
          if (cVar6 != -!bVar12) {
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
      }
    }
    if (cVar6 == '\x01') {
LAB_000c9406:
      candidate_5798c();
      uVar4 = 0;
      if (*(short *)((int)unaff_A6 + 0x6e) != 0) {
        uVar4 = 3;
      }
      *(undefined1 *)((int)unaff_A6 + 0x83) = uVar4;
      goto LAB_000c9420;
    }
    if (cVar6 != '\x02') {
      uVar7 = candidate_c9254();
      if ((DAT_0010c2e7 == '\0') && (uVar7 = candidate_57b44(), -1 < (short)uVar7)) {
        *(undefined1 *)(unaff_A6 + 0x18) = 0;
        bVar12 = false;
        candidate_40bc();
        if (bVar12) {
          sVar3 = *(short *)((int)unaff_A6 + 0x6e);
          *(short *)(extraout_A0_00 + 0x6e) = sVar3;
          if (sVar3 == 0) {
            uVar7 = 0x10c46e;
          }
          else {
            uVar7 = 0x10c476;
          }
          uVar2 = (uint)(ushort)(*(short *)(unaff_A6 + 0x1d) + 0x40U);
          *(ushort *)(extraout_A0_00 + 0x70) = *(short *)(unaff_A6 + 0x1d) + 0x40U;
          *(undefined4 *)(extraout_A0_00 + 0x72) = uVar7;
          *unaff_A6 = 0xc9a8c;
          if (*(char *)(unaff_A6 + 0x18) == '\0') goto LAB_000c9b30;
          candidate_c9254();
          candidate_4192();
        }
        uVar7 = candidate_57c82();
      }
      *(undefined1 *)(unaff_A6 + 0x22) = 3;
      *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
      *unaff_A6 = 0xc9ace;
      uVar1 = (undefined3)((uint)uVar7 >> 8);
      if (*(char *)(unaff_A6 + 0x20) == '\0') {
        bVar5 = -(*(char *)(unaff_A5 + 0x7db7) == '\x01') |
                -(*(char *)(unaff_A5 + 0x7db6) == '\x01');
        uVar2 = CONCAT31(uVar1,bVar5);
        if (bVar5 != 0) goto LAB_000c9312;
      }
      else {
        uVar8 = (short)CONCAT31(uVar1,*(char *)(unaff_A6 + 0x20)) - 1;
        uVar2 = (uint)uVar8;
        *(ushort *)(unaff_A6 + 0x20) = uVar8;
      }
      cVar6 = *(char *)(unaff_A5 + 0x69e1);
      *(char *)((int)unaff_A6 + 0x86) = cVar6;
      if (cVar6 != '\0') {
        uVar2 = candidate_3d25a();
      }
      *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(unaff_A5 + 0x69e1);
      goto LAB_000c9b30;
    }
  } while( true );
  while( true ) {
    uVar13 = candidate_c9202();
    uVar2 = (uint)(uVar13 >> 0x20);
    if (bVar12) break;
LAB_000c9554:
    uVar2 = (byte)((char)uVar13 + (char)(uVar13 >> 0x20)) & 0xffff0003;
    bVar12 = (byte)uVar2 < *(byte *)((int)unaff_A6 + 0x83);
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
