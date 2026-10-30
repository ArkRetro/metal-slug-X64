/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01c448.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1c448(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  char cVar6;
  undefined2 unaff_D2w;
  ushort uVar7;
  undefined2 unaff_D3w;
  undefined2 unaff_D4w;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined8 uVar8;

  if (((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) && (cVar6 = candidate_49e8(), cVar6 != '\0')) {
    if (cVar6 == '\x05') {
      candidate_120f8();
    }
    else {
      candidate_120f8();
    }
    candidate_1c318();
    unaff_A6[0xc] = -unaff_A6[0xc];
    if ((int)unaff_A6[0xd] < 0) {
      unaff_A6[0xd] = -unaff_A6[0xd];
    }
    candidate_411c();
    *(undefined1 *)(extraout_A0 + 0x82) = *(undefined1 *)((int)unaff_A6 + 0x82);
    candidate_1acf2();
  }
  *(undefined4 *)((int)unaff_A6 + 0x76) = unaff_A6[0xc];
  *(undefined4 *)((int)unaff_A6 + 0x7a) = unaff_A6[0xd];
  func_0x000143e4();
  *(undefined2 *)((int)unaff_A6 + 0x72) = unaff_D4w;
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) ||
     ((((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0 &&
       ((*(byte *)((int)unaff_A6 + 0x5f) & 2) == 0)) && ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)
      ))) {
    candidate_505a();
    uVar8 = candidate_1c6ac();
    *(undefined8 *)(unaff_A6 + 0xc) = uVar8;
    if (2 < *(short *)(unaff_A6 + 0xd)) {
      candidate_411c();
      *(undefined1 *)(extraout_A0_00 + 0x82) = *(undefined1 *)((int)unaff_A6 + 0x82);
      candidate_1acf2();
    }
    sVar4 = *(short *)(unaff_A6 + 0x1c) + -1;
    *(short *)(unaff_A6 + 0x1c) = sVar4;
    if (sVar4 == 0) {
      candidate_1c318();
    }
  }
  if (*(short *)(unaff_A6 + 0x1d) != 0) {
    sVar4 = *(short *)(unaff_A6 + 0x1d) + -1;
    *(short *)(unaff_A6 + 0x1d) = sVar4;
    if (sVar4 == 0) goto candidate_4188;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  unaff_A6[0xd] = (*(int *)((int)unaff_A6 + 0x7e) - unaff_A6[0xd] >> 4) + unaff_A6[0xd];
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
  candidate_427a();
  if ((*(ushort *)(unaff_A6 + 0xf) < 0x140) &&
     ((ushort)(*(short *)(unaff_A6 + 0x10) - 0x100U) < 0x140)) {
    return;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) {
    candidate_c0d48();
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
