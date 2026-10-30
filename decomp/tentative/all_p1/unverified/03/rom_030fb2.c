/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030fb2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_30fb2(void)

{
  code *pcVar1;
  ushort uVar2;
  short sVar3;
  ushort uVar4;
  ushort uVar5;
  short sVar6;
  int iVar7;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1abc8();
  sVar6 = *(short *)((int)unaff_A6 + 0x82);
  if (0xe < sVar6) {
    sVar6 = 0xe;
  }
  if (*(short *)(unaff_A6 + 0x20) == 0) {
    sVar6 = sVar6 * 8 + 0x1c;
  }
  else {
    sVar6 = sVar6 * -8 + 0x125;
  }
  *(short *)(unaff_A6 + 10) = (sVar6 - *(short *)(unaff_A6 + 0xf)) + *(short *)(unaff_A6 + 10);
  sVar6 = *(short *)(unaff_A6 + 0x10);
  *(short *)(unaff_A6 + 0xb) = (0x11c - sVar6) + *(short *)(unaff_A6 + 0xb);
  uVar5 = candidate_2eb4();
  sVar3 = (uVar5 & 0xff) * 2;
  iVar7 = *(short *)(sVar3 + 0x29fc) * 0x1000;
  unaff_A6[0xc] = *(short *)(sVar3 + 0x2a7c) * 0x1000;
  unaff_A6[0xd] = iVar7;
  candidate_1abc8();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x31060;
  if (*(char *)(unaff_A6[3] + 0x61) == '\0') {
    candidate_1ac38();
    candidate_427a();
    if (0x197 < *(short *)(unaff_A6 + 0x10)) {
      *unaff_A6 = 0x3108a;
    }
    return;
  }
  candidate_120f8();
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  uVar5 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
  if (0xfc10 < uVar5) {
    uVar5 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar2 = ~uVar5 & 0xfff;
    uVar5 = (ushort)CONCAT31((int3)((uint)iVar7 >> 8),
                             -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar2 & 7)) != 0));
    uRam002fffe0 = uVar5 & CONCAT11((char)((ushort)(0x11c - sVar6) >> 8),-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar5 ^ uVar4) & 1) != 0) {
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
  iVar7 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar7 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
