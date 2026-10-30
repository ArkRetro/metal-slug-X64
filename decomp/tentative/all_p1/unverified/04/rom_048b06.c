/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x048b06.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_48b06(void)

{
  code *pcVar1;
  int iVar2;
  char *pcVar3;
  char cVar4;
  ushort uVar5;
  short sVar6;
  ushort uVar7;
  uint uVar8;
  ushort uVar9;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;
  bool bVar11;
  undefined8 uVar12;

  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_1ad52();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0;
  uVar8 = candidate_29c6();
  if ((uVar8 & 1) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    candidate_1da40();
    candidate_40bc();
    candidate_1acf2();
    candidate_40bc();
    candidate_1acf2();
    candidate_40bc();
    candidate_1acf2();
    candidate_40bc();
    candidate_1acf2();
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    candidate_9f88();
    if (*(char *)(unaff_A6 + 0x18) != '\0') {
      pcVar3 = (char *)(*(int *)(unaff_A6[3] + 0xc) + 0x96);
      *pcVar3 = *pcVar3 + -1;
    }
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    candidate_120f8();
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0x1dec8;
    candidate_9ea8();
    candidate_1dea2();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
    unaff_A6[0x14] = 0;
    *unaff_A6 = 0x1c9e6;
    uVar10 = 0;
    candidate_1abc8();
    candidate_427a();
    if (!(bool)uVar10) {
      candidate_19504();
      if (!(bool)uVar10) {
        return;
      }
      uVar9 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
      if (0xfc10 < uVar9) {
        uVar9 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar7 = uRam002fffe8;
        uVar5 = ~uVar9 & 0xfff;
        uVar9 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7)
                           ) != 0));
        uRam002fffe0 = uVar9 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar9 ^ uVar7) & 1) != 0) {
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
  }
  else {
    uVar12 = candidate_2e7c();
    *(undefined8 *)(unaff_A6 + 0xc) = uVar12;
    candidate_1da40();
    candidate_40bc();
    candidate_1acf2();
    *(undefined2 *)(extraout_A0 + 0x66) = 0x1001;
    *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
    *(undefined2 *)(unaff_A6 + 0x25) = 5;
    *(undefined2 *)((int)unaff_A6 + 0x8e) = *(undefined2 *)(unaff_A6 + 0x25);
    uVar9 = candidate_29c6();
    *(ushort *)(unaff_A6 + 0x23) = (uVar9 & 0x1f) + 0x10;
    candidate_120f8();
    *unaff_A6 = 0x48bbc;
    func_0x000143e4();
    unaff_A6[0xd] = unaff_A6[0xd] + -0xe000;
    cVar4 = *(char *)((int)unaff_A6 + 0x96);
    if ('\x02' < cVar4) {
      *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + -3;
    }
    candidate_427a();
    *(char *)((int)unaff_A6 + 0x96) = cVar4;
    if ((int)unaff_A6[0xd] < 0) {
      bVar11 = false;
      candidate_1495c();
      if (bVar11) {
        *unaff_A6 = candidate_48c8c;
      }
    }
    sVar6 = *(short *)((int)unaff_A6 + 0x8e) + -1;
    *(short *)((int)unaff_A6 + 0x8e) = sVar6;
    if (sVar6 == 0) {
      *(undefined2 *)((int)unaff_A6 + 0x8e) = *(undefined2 *)(unaff_A6 + 0x25);
      candidate_40bc();
      candidate_1acf2();
      *(undefined2 *)(extraout_A0_00 + 0x66) = 0x1001;
      *(undefined2 *)(extraout_A0_00 + 0x1e) = 0x1001;
    }
    bVar11 = false;
    if (*(char *)(unaff_A6 + 0x23) != '\0') {
      sVar6 = *(short *)(unaff_A6 + 0x23);
      bVar11 = sVar6 == 0;
      sVar6 = sVar6 + -1;
      *(short *)(unaff_A6 + 0x23) = sVar6;
      if (sVar6 == 0) {
        uVar8 = candidate_29c6();
        bVar11 = false;
        if ((uVar8 & 1) == 0) {
          *unaff_A6 = candidate_48cd2;
          bVar11 = false;
        }
      }
    }
    candidate_1945c();
    if (!bVar11) {
      return;
    }
    if (*(char *)(unaff_A6 + 0x18) != '\0') {
      pcVar3 = (char *)(*(int *)(unaff_A6[3] + 0xc) + 0x96);
      *pcVar3 = *pcVar3 + -1;
    }
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
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
