/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06ef2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_6ef2a(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  ushort uVar4;
  ushort uVar5;
  undefined2 uVar7;
  uint uVar6;
  ushort uVar8;
  char cVar9;
  undefined1 extraout_D1b;
  undefined4 unaff_D2;
  undefined4 unaff_D3;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;
  bool bVar11;
  undefined8 uVar12;

  candidate_7005e();
  if (*(char *)(unaff_A6 + 0x21) != '\0') {
    candidate_4256();
    candidate_1b1dc();
    *unaff_A6 = 0x6ef56;
    uVar10 = 0;
    candidate_6fc28();
    candidate_6fc44();
    candidate_6fc20();
    if ((bool)uVar10) {
      *unaff_A6 = 0x6ef70;
    }
LAB_0006f568:
    if (*(short *)((int)unaff_A6 + 0x76) != 0) {
      *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    bVar3 = *(byte *)((int)unaff_A6 + 0x5f);
    *(byte *)((int)unaff_A6 + 0x5f) = bVar3 & 0xbf;
    if ((bVar3 & 0x40) != 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if (*(int *)((int)unaff_A6 + 0x6e) == 0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
      if ((*(ushort *)((int)unaff_A6 + 0x76) & 1) != 0) {
        *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
      }
    }
    else {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
      candidate_6ffa4();
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    if ((DAT_0010f000 & 0x80) != 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    bVar11 = false;
    candidate_aa760();
    if (bVar11) {
      cVar9 = candidate_aaa88();
      if (cVar9 == '\b') {
        *(undefined1 *)((int)unaff_A6 + 0x8a) = extraout_D1b;
        *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
      }
    }
    else {
      *unaff_A6 = candidate_6ee34;
    }
    if (*(char *)((int)unaff_A6 + 0x82) != '\0') {
      *(char *)((int)unaff_A6 + 0x82) = *(char *)((int)unaff_A6 + 0x82) + -1;
    }
    if (*(char *)((int)unaff_A6 + 0x83) != '\0') {
      *(char *)((int)unaff_A6 + 0x83) = *(char *)((int)unaff_A6 + 0x83) + -1;
    }
    bVar11 = false;
    candidate_aa9e4();
    if (bVar11) {
      *(undefined2 *)((int)unaff_A6 + 0x76) = 0x1e;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      uVar12 = candidate_2e7c();
      unaff_A6[0xc] = (int)((ulonglong)uVar12 >> 0x20) + unaff_A6[0xc];
      unaff_A6[0xd] = (int)uVar12 + unaff_A6[0xd];
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_6f326;
    }
    goto LAB_0006f674;
  }
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x22) = 0;
  unaff_A6[0xd] = 0x80000;
  *unaff_A6 = 0x6efa6;
  candidate_6fe22();
  candidate_6fc28();
  candidate_6fd1e();
  if (1 < *(short *)(unaff_A6 + 0xd)) {
    candidate_143ee();
    candidate_6fd88();
    candidate_6fe46();
    candidate_6fc20();
    goto LAB_0006f568;
  }
  while( true ) {
    *(undefined1 *)((int)unaff_A6 + 0x82) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
    *unaff_A6 = 0x6f0f2;
    candidate_6fe22();
    uVar6 = candidate_6fc28();
    if (((uVar6 & 2) == 0) || ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) break;
    while( true ) {
      candidate_4256();
      candidate_1b1dc();
      *(undefined1 *)((int)unaff_A6 + 0x82) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
      *unaff_A6 = 0x6f000;
      candidate_6fe22();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
        uVar6 = candidate_6fc28();
        if ((uVar6 & 0x20) != 0) {
          uVar6 = candidate_29c6();
          if ((uVar6 & 7) == 0) {
            bVar11 = false;
            candidate_40bc();
            if (bVar11) {
              candidate_1b220();
            }
          }
          *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
          *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            candidate_4256();
            candidate_1b1dc();
            *unaff_A6 = 0x6ee68;
            uVar10 = 0;
            candidate_6fcca();
            candidate_1945c();
            if ((bool)uVar10) {
              *unaff_A6 = candidate_4188;
            }
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
              candidate_6fc20();
              goto LAB_0006f674;
            }
          }
          candidate_4256();
          candidate_1b1dc();
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
          candidate_353a4();
          *unaff_A6 = 0x6eec2;
          uVar10 = 0;
          candidate_6fcca();
          candidate_6fc20();
          candidate_1945c();
          if ((bool)uVar10) {
            *unaff_A6 = candidate_4188;
            uVar10 = 0;
          }
          uVar7 = candidate_aa610();
          if ((bool)uVar10) {
            *(undefined2 *)((int)unaff_A6 + 0x72) = uVar7;
            bVar11 = false;
            uVar7 = candidate_aa760();
            if (bVar11) {
              *(undefined2 *)(unaff_A6 + 0x1d) = uVar7;
              candidate_aa83a();
              *unaff_A6 = candidate_6ef2a;
            }
          }
          goto LAB_0006f674;
        }
        if ((uVar6 & 2) != 0) {
          candidate_6fd1e();
          candidate_143ee();
          candidate_6fc20();
          candidate_6ff12();
          goto LAB_0006f568;
        }
      }
      if (*(char *)(unaff_A6 + 0x21) == '\0') break;
      candidate_4256();
      candidate_1b1dc();
      *unaff_A6 = 0x6f08e;
      uVar10 = 0;
      uVar6 = candidate_6fc28();
      if (((uVar6 & 2) == 0) || ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
        candidate_6fc44();
        candidate_6fc20();
        if ((bool)uVar10) {
          *unaff_A6 = 0x6f0ba;
        }
        goto LAB_0006f568;
      }
    }
    candidate_4256();
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  if ((uVar6 & 0x80) == 0) {
    candidate_6fd1e();
    candidate_6fd88();
    candidate_6fc74();
    candidate_6fe46();
    candidate_6fc20();
    candidate_6feb0();
    goto LAB_0006f568;
  }
  candidate_aa7fc();
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xe29b2;
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
  *unaff_A6 = 0x6f168;
  if (*(int *)((int)unaff_A6 + 0x6e) != 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_6ffa4();
  }
  uVar8 = candidate_49e8();
  if (5 < uVar8) goto LAB_0006f1ae;
                    /* WARNING: Switch is manually overridden */
  switch(uVar8 << 2) {
  case 0:
    break;
  case 4:
    break;
  case 8:
    goto LAB_0006f20e;
  case 0xc:
    goto LAB_0006f20e;
  case 0x10:
    break;
  case 0x14:
  }
LAB_0006f1ae:
  if ((*(short *)(unaff_A6 + 0xf) < 0x111) && ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
    unaff_A6[0xc] = (0x100000 - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
    unaff_A6[0xd] = unaff_A6[0xd] + -0x800;
    bVar11 = false;
    candidate_1495c();
    if ((bVar11) && ((int)unaff_A6[0xd] < 0)) {
      unaff_A6[0xd] = -unaff_A6[0xd];
    }
    candidate_143ee();
    candidate_6fd88();
    candidate_6fe46();
    candidate_6fc20();
    *(undefined2 *)(unaff_A6 + 0x1e) = 0x8000;
LAB_0006f674:
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    return;
  }
LAB_0006f20e:
  candidate_49e8();
  candidate_ae220();
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0 + 0x30) = 0xfff30000;
  *(undefined4 *)(extraout_A0 + 0x34) = 0x180000;
  *(undefined2 *)(extraout_A0 + 0x4e) = 10;
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0_00 + 0x30) = 0xb0000;
  *(undefined4 *)(extraout_A0_00 + 0x34) = 0x150000;
  *(undefined2 *)(extraout_A0_00 + 0x4e) = 7;
  candidate_ad9e6();
  *(undefined4 *)(extraout_A0_01 + 0x30) = 0x80000;
  *(undefined4 *)(extraout_A0_01 + 0x34) = 0xfffc0000;
  *(undefined2 *)(extraout_A0_01 + 0x4e) = 5;
  candidate_411c();
  candidate_1acf2();
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar4 = ~uVar8 & 0xfff;
    uVar8 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                             -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar4 & 7)) != 0));
    uRam002fffe0 = uVar8 & (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar4));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar8 ^ uVar5) & 1) != 0) {
      *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
      _DAT_002ffff0 = 1;
      do {
      } while (DAT_002fffff != '\x01');
      uRam00400000 = 0;
      uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
      uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
      uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
      uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
      uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
      uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
      uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
      uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
      uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
      uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
      uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
      uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
      uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
      uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
      uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
      _DAT_00401ffe = 0;
      func_0x0003cf12();
      candidate_3e2a8();
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
