/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9a3e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_d9a3e(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  undefined2 unaff_D2w;
  ushort uVar6;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar7;
  undefined2 in_stack_fffffffa;
  undefined4 uVar8;
  undefined2 uVar9;

  *(undefined1 *)((int)unaff_A6 + 0x7b) = 0;
  if (((char)*(undefined2 *)(unaff_A6 + 0x22) >> 3 & 1U) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x32;
  candidate_4256();
  *unaff_A6 = 0xd9a74;
  uVar9 = 0x9a7a;
  candidate_1abc8();
  uVar8 = CONCAT22(*(undefined2 *)(unaff_A6 + 10),in_stack_fffffffa);
  candidate_da40a(uVar8,CONCAT22(*(undefined2 *)(unaff_A6 + 0xb),uVar9));
  candidate_427a();
  sVar4 = *(short *)((int)unaff_A6 + 0x7e) + -1;
  *(short *)((int)unaff_A6 + 0x7e) = sVar4;
  if (sVar4 == 0) {
    candidate_da464();
    *unaff_A6 = 0xd97c0;
  }
  *(short *)(unaff_A6 + 10) = (short)((uint)uVar8 >> 0x10);
  *(short *)(unaff_A6 + 0xb) = (short)uVar8;
  candidate_1abc8();
  uVar7 = *(char *)((int)unaff_A6 + 0x61) != -1;
  if (*(char *)((int)unaff_A6 + 0x61) == -1) {
    if (*(char *)(unaff_A6 + 0x18) == -0x78) {
      *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
    }
    *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
    *unaff_A6 = candidate_b0626;
    uVar7 = 0;
    candidate_da40a();
    candidate_da23e();
  }
  candidate_da21e();
  if (!(bool)uVar7) {
    candidate_da370();
    if (!(bool)uVar7) {
      return;
    }
  }
  *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
  candidate_da23e();
  uVar6 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
  if (0xfc10 < uVar6) {
    uVar6 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar3 = ~uVar6 & 0xfff;
    uVar6 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)) !=
                      0));
    uRam002fffe0 = uVar6 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar6 ^ uVar5) & 1) != 0) {
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
