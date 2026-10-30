/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07497c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_7497c(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  ushort uVar5;
  ushort uVar6;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar7;

  sVar4 = *(short *)((int)unaff_A6 + 0x82) + -1;
  *(short *)((int)unaff_A6 + 0x82) = sVar4;
  if (sVar4 < 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe000;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe000;
  }
  unaff_A6[0xc] = unaff_A6[0xc] + 0x10000;
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) goto LAB_000749f4;
  uVar6 = candidate_49e8();
  bVar7 = uVar6 < 6;
  if (!bVar7) goto LAB_000749e2;
  bVar7 = CARRY2(uVar6 * 2,uVar6 * 2);
                    /* WARNING: Switch is manually overridden */
  switch(uVar6 << 2) {
  case 0:
    break;
  case 4:
    break;
  case 8:
    goto LAB_000749f4;
  case 0xc:
LAB_000749f4:
    candidate_49e8();
    candidate_411c();
    *(undefined2 *)(extraout_A0 + 0x66) = 0x1001;
    *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
    candidate_29c6();
    candidate_1acf2();
    goto LAB_00074a46;
  case 0x10:
    break;
  case 0x14:
  }
LAB_000749e2:
  candidate_19504();
  if (!bVar7) {
    return;
  }
LAB_00074a46:
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
