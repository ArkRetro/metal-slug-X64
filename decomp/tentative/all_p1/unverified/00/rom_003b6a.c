/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x003b6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_3b6a(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint *puVar7;
  uint *puVar8;
  uint *puVar9;
  int unaff_A5;
  bool bVar10;

  uRam002fffea = 0xa0;
  *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
  _DAT_002ffff0 = 1;
  do {
  } while (DAT_002fffff != '\x01');
  puVar6 = (uint *)0xdedd2;
  sVar1 = 0x40;
  while (bVar10 = sVar1 != 0, sVar1 = sVar1 + -1, bVar10) {
    puVar6 = puVar6 + 1;
    sVar3 = 0x1f;
    do {
      _DAT_0010f00a = uRam002fffe8 + _DAT_0010f00a;
      bVar10 = sVar3 != 0;
      sVar3 = sVar3 + -1;
    } while (bVar10);
  }
  uRam002fffe6 = 0;
  uVar2 = 0;
  puVar8 = (uint *)0xdedd2;
  do {
    puVar7 = puVar6 + 1;
    uVar5 = *puVar6;
    puVar9 = puVar8 + 1;
    uVar4 = *puVar8;
    do {
      uVar4 = uVar4 << 1 | uVar4 >> 0x1f;
      uVar5 = uVar5 << 1 | uVar5 >> 0x1f;
      uRam002fffe0 = (ushort)uVar4 ^ _DAT_0010f00a;
      _DAT_0010f00a = uRam002fffe0 + _DAT_0010f00a;
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      if ((((ushort)uVar5 ^ uRam002fffe8) & 1) != 0) {
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
      uVar2 = uVar2 + 1;
      if (0x420 < uVar2) {
        uRam002fffe6 = 0;
        _DAT_0010f00a = _DAT_0010f00a & 0xf000;
        return;
      }
      puVar6 = puVar7;
      puVar8 = puVar9;
    } while ((uVar2 & 0x1f) != 0);
  } while( true );
}
