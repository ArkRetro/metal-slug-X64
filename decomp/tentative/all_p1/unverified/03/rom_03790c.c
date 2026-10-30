/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03790c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_3790c(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar5;
  int iVar4;
  undefined2 uVar6;
  undefined2 unaff_D2w;
  ushort uVar7;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;

  *(undefined1 *)((int)unaff_A6 + 0x97) = 1;
  candidate_375a8();
  candidate_374ba();
  candidate_9ea8();
  sVar5 = candidate_29c6();
  if (sVar5 < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  sVar5 = candidate_29f2();
  iVar4 = (short)(sVar5 + 0x300) * 0x100;
  uVar6 = 0x61;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar4 = (short)(sVar5 + 0x300) * -0x100;
    uVar6 = 0x1ef;
  }
  unaff_A6[0xc] = iVar4;
  *(undefined2 *)(unaff_A6 + 10) = uVar6;
  candidate_4256();
  candidate_37616();
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffffffff;
  *(undefined1 *)(unaff_A6 + 0x26) = 6;
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  *unaff_A6 = 0x37996;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_3775e();
    candidate_377f8();
    sVar5 = *(short *)((int)unaff_A6 + 0x8a);
    uVar8 = sVar5 == 0;
    sVar5 = sVar5 + -1;
    *(short *)((int)unaff_A6 + 0x8a) = sVar5;
    if (sVar5 < 0) {
      candidate_37616();
      candidate_3788e();
    }
    candidate_378d2();
    candidate_3786c();
    candidate_1ac38();
    candidate_427a();
    candidate_37c0a();
    if ((!(bool)uVar8) && (candidate_384e0(), (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
      *unaff_A6 = 0x37a86;
      uVar8 = 0;
    }
    candidate_374d8();
    if (!(bool)uVar8) {
      return;
    }
    candidate_37acc();
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar2 & 7))
                        != 0));
      uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar7 ^ uVar3) & 1) != 0) {
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
  else {
    candidate_37acc();
    candidate_9f88();
    candidate_9ea8();
    candidate_4256();
    *unaff_A6 = 0x37ab4;
    uVar8 = 0;
    candidate_1abc8();
    candidate_427a();
    if (!(bool)uVar8) {
      return;
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
