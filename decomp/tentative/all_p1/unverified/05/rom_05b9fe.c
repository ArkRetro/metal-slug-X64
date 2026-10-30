/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05b9fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_5b9fe(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  undefined2 unaff_D2w;
  ushort uVar4;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int iVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  candidate_1ad52();
  *(undefined2 *)(unaff_A6 + 0xb) = 0x120;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xffff;
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  if (((*(byte *)(unaff_A6 + 0x21) & 4) == 0) && (*(char *)((int)unaff_A6 + 0x83) == '\0')) {
    candidate_c8022();
  }
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_120f8();
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  candidate_1da40();
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  bVar6 = false;
  candidate_40bc();
  iVar5 = extraout_A0;
  if (bVar6) {
    candidate_1b220();
    iVar5 = extraout_A0_00;
  }
  *(byte *)(iVar5 + 0x15) = *(byte *)(iVar5 + 0x15) ^ 1;
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  uVar4 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar4 + 0x3ef;
  if (0xfc10 < uVar4) {
    uVar4 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar3 = uRam002fffe8;
    uVar2 = ~uVar4 & 0xfff;
    uVar4 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar4 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7)) !=
                      0));
    uRam002fffe0 = uVar4 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar4 ^ uVar3) & 1) != 0) {
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
  iVar5 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar5 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
