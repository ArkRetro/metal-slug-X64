/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08c374.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8c374(void)

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
  int extraout_A0;
  short *psVar8;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;
  undefined1 uVar10;

  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
  if (*(short *)((int)unaff_A6 + 0x6e) == -1) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar5 = uRam002fffe8;
      uVar3 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7))
                        != 0));
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
    goto candidate_4188;
  }
  if (*(char *)((int)unaff_A6 + 0x7f) != '\0') {
    candidate_146e8();
  }
  if ((*(byte *)((int)unaff_A6 + 0x73) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  if ((*(byte *)((int)unaff_A6 + 0x73) & 2) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 2;
  }
  if ((*(byte *)((int)unaff_A6 + 0x73) & 0x10) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  }
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8838;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8838;
  *(undefined1 *)(unaff_A6 + 0x20) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
  *(undefined2 *)(unaff_A6 + 0x1e) = 5;
  candidate_8d2da();
  *(int *)((int)unaff_A6 + 0x82) = extraout_A0;
  candidate_9ea8(extraout_A0 + 8);
  candidate_9ea8();
  candidate_9ea8();
  candidate_8cbca();
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x78;
  if (*(short *)(unaff_A6 + 0x1c) == 0) {
    *(undefined2 *)(unaff_A6 + 0x1c) = 600;
  }
  if ((*(char *)((int)unaff_A6 + 0x8a) == '\0') &&
     (*(char *)(*(int *)((int)unaff_A6 + 0x82) + 2) != '\0')) {
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  }
  else {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  candidate_1abc8();
  *unaff_A6 = 0x8c53c;
  if ((*(short *)(unaff_A6 + 0x1c) != -1) &&
     (sVar4 = *(short *)(unaff_A6 + 0x1c) + -1, *(short *)(unaff_A6 + 0x1c) = sVar4, sVar4 == 0)) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    *(undefined2 *)(unaff_A6 + 0x1c) = 0x1e;
    *unaff_A6 = candidate_8c564;
  }
  sVar4 = *(short *)((int)unaff_A6 + 0x86);
  *(short *)((int)unaff_A6 + 0x86) = sVar4 + -1;
  if (sVar4 == 0) {
    *(undefined1 *)(unaff_A6 + 0x1f) = 0xff;
  }
  (**(code **)(*(int *)((int)unaff_A6 + 0x82) + 0x14))();
  bVar9 = false;
  if (*(char *)(unaff_A6 + 0x20) == '\0') {
    if (*(short *)(unaff_A6 + 0x1e) != 0) {
      sVar4 = *(short *)(unaff_A6 + 0x1e);
      bVar9 = sVar4 == 0;
      *(short *)(unaff_A6 + 0x1e) = sVar4 + -1;
      goto LAB_0008c5b8;
    }
    cVar6 = candidate_8c0cc();
    bVar9 = false;
    if (cVar6 < '\0') goto LAB_0008c5b8;
    (**(code **)(*(int *)((int)unaff_A6 + 0x82) + 0x10))();
    candidate_8cbf8();
    if (*(char *)(*(int *)((int)unaff_A6 + 0x82) + 2) != '\0') {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
      candidate_411c();
      candidate_1acf2();
    }
    *(undefined1 *)(unaff_A6 + 0x23) = 2;
    *unaff_A6 = 0x8c66a;
    (**(code **)(*(int *)((int)unaff_A6 + 0x82) + 0x14))();
    candidate_427a();
    cVar6 = *(char *)(unaff_A6 + 0x23) + -1;
    *(char *)(unaff_A6 + 0x23) = cVar6;
    if (cVar6 != '\0') {
      return;
    }
  }
  else {
LAB_0008c5b8:
    candidate_427a();
    if (bVar9) {
      if (*(char *)((int)unaff_A6 + 0x8a) != '\0') {
        psVar8 = (short *)0xe553e;
        do {
          sVar4 = *psVar8;
          if (sVar4 < 0) goto LAB_0008c602;
          psVar8 = psVar8 + 1;
        } while (*(short *)((int)unaff_A6 + 0x6e) != sVar4);
        goto LAB_0008c684;
      }
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
LAB_0008c602:
    uVar10 = false;
    if (((*(char *)((int)unaff_A6 + 0x8b) != '\0') && (candidate_409a(), (bool)uVar10)) ||
       (candidate_c8bec(), !(bool)uVar10)) {
      return;
    }
  }
LAB_0008c684:
  candidate_8cbf8();
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
