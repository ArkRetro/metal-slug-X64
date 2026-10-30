/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072110.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_72110(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  undefined1 uVar6;
  ushort uVar7;
  undefined2 unaff_D2w;
  undefined1 uVar8;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;
  undefined1 uVar10;

  candidate_9ea8();
  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x89) = 0;
  *(undefined1 *)(unaff_A6 + 0x21) = 0xff;
  *unaff_A6 = 0x7213e;
  bVar9 = false;
  candidate_409a();
  uVar6 = (undefined1)((ushort)unaff_D2w >> 8);
  uVar8 = (undefined1)((ushort)unaff_D3w >> 8);
  if (bVar9) {
    candidate_1aca2();
    if (*(char *)(extraout_A0 + 0x84) < '\0') {
      return;
    }
    candidate_72298();
    if (*(char *)(extraout_A0_00 + 0x7f) == '\0') {
      if (*(char *)(unaff_A6[3] + 0x84) != *(char *)(unaff_A6 + 0x21)) {
        *(char *)(unaff_A6 + 0x21) = *(char *)(unaff_A6[3] + 0x84);
        candidate_4256();
      }
      candidate_7227c();
      if (*(char *)(unaff_A6 + 0x21) == '\b') {
        *(short *)(unaff_A6 + 0xf) = *(short *)(unaff_A6 + 0xf) + 0x28;
        sVar5 = candidate_722b2();
        if (sVar5 != 0) {
          *(undefined1 *)(unaff_A6[3] + 0x8c) = 2;
        }
      }
      return;
    }
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
    *(undefined1 *)(unaff_A6 + 0x21) = 0xff;
    *(undefined2 *)((int)unaff_A6 + 0x76) = 0x1e;
    *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
    *unaff_A6 = 0x721dc;
    bVar9 = false;
    candidate_409a();
    if (bVar9) {
      candidate_72770();
      candidate_1ad6e();
      candidate_1aca2();
      candidate_72298();
      sVar5 = *(short *)((int)unaff_A6 + 0x76);
      uVar10 = sVar5 == 0;
      sVar5 = sVar5 + -1;
      *(short *)((int)unaff_A6 + 0x76) = sVar5;
      if ((-1 < sVar5) && ((*(byte *)(unaff_A6[3] + 0x7c) & 4) == 0)) {
        candidate_722fe();
        candidate_7227c();
        if (!(bool)uVar10) {
          *(undefined1 *)(unaff_A6[3] + 0x7f) = 3;
          return;
        }
      }
      candidate_4256();
      *unaff_A6 = 0x72250;
      bVar9 = false;
      candidate_409a();
      if (bVar9) {
        *(undefined1 *)(unaff_A6[3] + 0x7f) = 3;
        bVar9 = false;
        candidate_7227c();
        if (bVar9) {
          *(undefined1 *)(unaff_A6[3] + 0x7f) = 0;
          *unaff_A6 = 0x7212c;
        }
        return;
      }
    }
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (uVar7 < 0xfc11) goto candidate_4188;
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11(uVar6,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11(uVar8,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar7 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4);
  }
  else {
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (uVar7 < 0xfc11) goto candidate_4188;
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11(uVar6,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11(uVar8,-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar7 = (ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4);
  }
  if ((uVar7 & 1) != 0) {
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
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
