/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ab10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_4ab10(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  short sVar5;
  ushort uVar6;
  ushort uVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar9;

  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) & 0x7f;
  *(undefined4 *)((int)unaff_A6 + 0x7a) = 0x20000;
  candidate_1da40();
  candidate_1da40();
  candidate_c801a();
  candidate_120f8();
  sVar5 = *(short *)((short)(*(short *)((int)unaff_A6 + 0x86) * 2) + 0x4ad0c);
  if (sVar5 == -1) {
    *(undefined2 *)(unaff_A6 + 0x23) = 0x7fff;
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x23) = *(undefined2 *)(unaff_A6 + 0xf);
    *(short *)(unaff_A6 + 0x23) = *(short *)(unaff_A6 + 0x23) - sVar5;
  }
  *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + 1;
  unaff_A6[0xc] = 0;
  *unaff_A6 = 0x4ab82;
  if (((*(byte *)(unaff_A6 + 0x18) & 0x80) == 0) && (*(short *)(unaff_A6 + 0xf) < 0x11)) {
    *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 0x80;
    candidate_1da40();
    uVar3 = unaff_A6[0xf];
    uVar4 = unaff_A6[0x10];
    unaff_A6[0xf] = unaff_A6[0xf] + 0x200000;
    unaff_A6[0x10] = unaff_A6[0x10] + 0x800000;
    candidate_1db74();
    unaff_A6[0x10] = uVar4;
    unaff_A6[0xf] = uVar3;
    candidate_c801a();
    candidate_120f8();
  }
  if (*(short *)(unaff_A6 + 0x23) == 0x7fff) {
    unaff_A6[0xc] = unaff_A6[0xc] + -0x2000;
  }
  else {
    unaff_A6[0xc] = 0xfffe0000;
    if (*(short *)(unaff_A6 + 0xf) <= *(short *)(unaff_A6 + 0x23)) {
      *unaff_A6 = candidate_4accc;
    }
  }
  *(int *)((int)unaff_A6 + 0x7a) = -*(int *)((int)unaff_A6 + 0x7a);
  unaff_A6[0xd] = *(undefined4 *)((int)unaff_A6 + 0x7a);
  uVar9 = 0;
  candidate_1ac38();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar9) {
    candidate_18f40();
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar7 = uRam002fffe8;
      uVar6 = ~uVar8 & 0xfff;
      uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar6 & 7))
                        != 0));
      uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar6));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar8 ^ uVar7) & 1) != 0) {
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
  return;
}
