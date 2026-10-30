/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08b18e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0008b18e(void)

{
  code *pcVar1;
  uint uVar2;
  ushort uVar3;
  uint uVar4;
  ushort uVar5;
  int iVar6;
  undefined2 unaff_D2w;
  ushort uVar7;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;

  *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) | 2;
                    /* WARNING: Switch is manually overridden */
  switch(*(short *)((int)unaff_A6 + 0x6e) * 4) {
  case 0:
    candidate_4256();
    *(undefined2 *)(unaff_A6 + 0x1f) = 0x10;
    *unaff_A6 = candidate_8b1c4;
    uVar7 = *(ushort *)(unaff_A6 + 0x1f);
    uVar8 = uVar7 == 0;
    uVar7 = uVar7 - 1;
    *(ushort *)(unaff_A6 + 0x1f) = uVar7;
    if ((int)((uint)uVar7 << 0x10) < 0) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) != 0) {
        uVar7 = *(ushort *)(unaff_A5 + 0x700c);
        *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
        if (0xfc10 < uVar7) {
          uVar7 = *(ushort *)(unaff_A5 + 0x700a);
          *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
          uVar5 = uRam002fffe8;
          uVar3 = ~uVar7 & 0xfff;
          uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                           -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar3 & 7)) != 0));
          uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
          uRam002fffe2 = 0;
          uRam002fffe4 = 0;
          uRam002fffe6 = 0;
          *(short *)(unaff_A5 + 0x700a) =
               ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU)
               + *(short *)(unaff_A5 + 0x700a);
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
        iVar6 = *(int *)(unaff_A5 + -0x1c24);
        do {
          unaff_A6 = (undefined4 *)unaff_A6[2];
          pcVar1 = (code *)*unaff_A6;
          *(undefined4 *)(iVar6 + -4) = 0x424a;
          (*pcVar1)();
        } while( true );
      }
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
      *(undefined2 *)(unaff_A6 + 0x1f) = 8;
      uVar8 = false;
    }
    candidate_8b31a();
    candidate_427a();
    break;
  case 4:
    candidate_1ad52();
    candidate_4256();
    unaff_A6[0xc] = 0;
    *unaff_A6 = candidate_8b214;
    iVar6 = 0x20000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      iVar6 = -0x20000;
    }
    unaff_A6[0xc] = (iVar6 - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
    uVar4 = 0x40000 - unaff_A6[0xd] >> 4;
    uVar2 = unaff_A6[0xd];
    uVar8 = CARRY4(uVar4,uVar2);
    unaff_A6[0xd] = uVar4 + uVar2;
    candidate_1ac38();
    candidate_427a();
  }
  candidate_1945c();
  if ((bool)uVar8) {
    *unaff_A6 = 0x8b250;
  }
  return;
}
