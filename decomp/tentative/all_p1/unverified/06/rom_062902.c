/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x062902.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_62902(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  short sVar6;
  undefined4 extraout_D1;
  undefined2 unaff_D2w;
  ushort uVar7;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;
  undefined2 in_stack_fffffffa;
  undefined4 uVar9;
  undefined2 uVar10;

  candidate_1b1dc();
  *unaff_A6 = 0x6290e;
  *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + '\n';
  candidate_2e7c();
  unaff_A6[0xd] = extraout_D1;
  candidate_1ac38();
  candidate_427a();
  sVar6 = candidate_49e8();
  if (sVar6 != 0) {
    *unaff_A6 = 0x62be4;
  }
  sVar6 = *(short *)(unaff_A6 + 0x21);
  sVar4 = sVar6 + -1;
  *(short *)(unaff_A6 + 0x21) = sVar4;
  if (sVar4 == 0 || 0 >= sVar6) {
    *unaff_A6 = candidate_62962;
  }
  bVar8 = (sVar4 != 0 && 0 < sVar6) && sVar6 == 0;
  candidate_1945c();
  if (!bVar8) {
    uVar10 = 0x2a02;
    candidate_4c24();
    if (bVar8) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar9 = CONCAT22(unaff_D2w,in_stack_fffffffa);
      candidate_40bc(uVar9,CONCAT22(unaff_D3w,uVar10));
      *(short *)(extraout_A0 + 0x28) = (short)((uint)uVar9 >> 0x10);
      *(short *)(extraout_A0 + 0x2a) = (short)uVar9;
      candidate_1ac94();
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_62a40;
    }
    return;
  }
  uVar7 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
  if (0xfc10 < uVar7) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)) !=
                      0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar5) & 1) != 0) {
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
