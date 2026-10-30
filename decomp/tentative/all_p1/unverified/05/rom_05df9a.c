/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05df9a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_5df9a(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar5;
  int iVar4;
  ushort uVar6;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar7;
  char cVar8;
  bool bVar9;

  sVar5 = candidate_260c8();
  iVar4 = sVar5 * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar4 = sVar5 * -0x100;
  }
  unaff_A6[0xc] = iVar4;
  *(undefined2 *)(unaff_A6 + 0xd) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x96;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_120f8();
  candidate_4256();
  *unaff_A6 = 0x5e03e;
  func_0x000143e4();
  cVar8 = '\0';
  if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
    candidate_3c5c6();
    *(char *)((int)unaff_A6 + 0x92) = -cVar8;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) == 0) {
    candidate_427a();
    *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
    if (*(short *)((int)unaff_A6 + 0x7a) < 1) {
      *unaff_A6 = candidate_5dd12;
    }
    sVar5 = candidate_49e8();
    if (sVar5 != 0) {
      *unaff_A6 = candidate_5dd12;
    }
    bVar9 = false;
    candidate_19504();
    if (bVar9) {
      *unaff_A6 = candidate_41ac;
    }
    return;
  }
  if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    candidate_9f88();
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0x1deb8;
    candidate_9ea8();
    candidate_1dea2();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
    unaff_A6[0x14] = 0;
    *unaff_A6 = 0x1c9e6;
    uVar7 = 0;
    candidate_1abc8();
    candidate_427a();
    if (!(bool)uVar7) {
      candidate_19504();
      if (!(bool)uVar7) {
        return;
      }
      uVar6 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
      if (0xfc10 < uVar6) {
        uVar6 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar3 = uRam002fffe8;
        uVar2 = ~uVar6 & 0xfff;
        uVar6 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)
                           ) != 0));
        uRam002fffe0 = uVar6 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar6 ^ uVar3) & 1) != 0) {
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
    }
  }
  candidate_4126();
  iVar4 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar4 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
