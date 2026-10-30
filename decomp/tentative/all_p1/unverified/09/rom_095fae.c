/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x095fae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_95fae(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  undefined2 unaff_D2w;
  ushort uVar6;
  undefined2 unaff_D3w;
  short sVar7;
  undefined4 *puVar8;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;
  undefined1 uVar10;
  short *psVar11;
  short *psVar12;
  int *piVar13;

  candidate_1abc8();
  puVar8 = unaff_A6 + 0x1f;
  sVar7 = 0xc;
  sVar5 = 4;
  bVar9 = false;
  while (!bVar9) {
    *(undefined4 **)((int)register0x0000003c + -4) = puVar8;
    *(short *)((int)register0x0000003c + -8) = sVar7;
    psVar11 = (short *)((int)register0x0000003c + -0xc);
    *psVar11 = sVar5;
    unaff_D2w = *(undefined2 *)puVar8;
    unaff_D3w = 0xffff;
    *(undefined4 *)((int)register0x0000003c + -0x10) = 0x95fde;
    candidate_8cd70();
    psVar12 = (short *)((int)register0x0000003c + -10);
    piVar13 = (int *)((int)register0x0000003c + -8);
    register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
    puVar8 = (undefined4 *)(*piVar13 + 2);
    sVar7 = *psVar12 + 0x18;
    sVar5 = *psVar11 + -1;
    bVar9 = sVar5 == 0;
  }
  *(undefined2 *)((int)unaff_A6 + 0x8a) = 0;
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  *unaff_A6 = 0x95ffc;
  uVar10 = 0;
  *(undefined4 *)((int)register0x0000003c + -4) = 0x96002;
  candidate_1abc8();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x96008;
  candidate_409a();
  if ((bool)uVar10) {
    *(undefined4 *)((int)register0x0000003c + -4) = 0x96018;
    candidate_408c();
    if ((bool)uVar10) {
      if (*(char *)(unaff_A6[3] + 0x96) != '\0') {
        *(undefined1 *)(unaff_A6[3] + 0x96) = 0;
        *(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + 0xc;
        *(ushort *)((int)unaff_A6 + 0x8a) = *(ushort *)((int)unaff_A6 + 0x8a) & 0xf8;
      }
      sVar5 = *(short *)((int)unaff_A6 + 0x6e) << 2;
      sVar7 = *(short *)(sVar5 + 0x95b46);
      sVar5 = *(short *)(sVar5 + 0x95b44);
      uVar6 = *(ushort *)((int)unaff_A6 + 0x8a);
      if (uVar6 != *(ushort *)(unaff_A6 + 0x23)) {
        if ((short)*(ushort *)(unaff_A6 + 0x23) < (short)uVar6) {
          *(short *)(unaff_A6 + 0x23) = *(short *)(unaff_A6 + 0x23) + 2;
          if (uVar6 < *(ushort *)(unaff_A6 + 0x23)) {
            *(ushort *)(unaff_A6 + 0x23) = uVar6;
          }
        }
        else {
          *(short *)(unaff_A6 + 0x23) = *(short *)(unaff_A6 + 0x23) + -1;
        }
      }
      uVar6 = *(ushort *)(unaff_A6 + 0x23);
      if (*(ushort *)(unaff_A6 + 0x1e) <= uVar6) {
        *unaff_A6 = 0x960a6;
      }
      *(undefined4 *)((int)register0x0000003c + -4) = unaff_A6[10];
      *(undefined4 *)((int)register0x0000003c + -8) = unaff_A6[0xb];
      *(ushort *)(unaff_A6 + 10) = sVar5 * uVar6 + *(short *)(unaff_A6 + 10);
      *(ushort *)(unaff_A6 + 0xb) = sVar7 * uVar6 + *(short *)(unaff_A6 + 0xb);
      *(undefined4 *)((int)register0x0000003c + -0xc) = 0x9609c;
      func_0x00095f98();
      unaff_A6[0xb] = *(undefined4 *)((int)register0x0000003c + -8);
      unaff_A6[10] = *(undefined4 *)((int)register0x0000003c + -4);
      return;
    }
    *(undefined1 *)(unaff_A6[3] + 0x95) = 0xff;
    unaff_A6[0xc] = 0;
    uVar6 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
    if (0xfc10 < uVar6) {
      uVar6 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar4 = uRam002fffe8;
      uVar3 = ~uVar6 & 0xfff;
      uVar6 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7))
                        != 0));
      uRam002fffe0 = uVar6 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar6 ^ uVar4) & 1) != 0) {
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
        *(undefined4 *)((int)register0x0000003c + -4) = 0x3d2c;
        func_0x0003cf12();
        *(undefined4 *)((int)register0x0000003c + -4) = 0x3d36;
        candidate_3e2a8();
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
    }
  }
  else {
    *(undefined4 *)((int)register0x0000003c + -4) = 0x41ae;
    candidate_4192();
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0x418a;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
