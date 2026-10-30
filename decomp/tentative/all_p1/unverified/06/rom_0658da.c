/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0658da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_658da(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  char cVar5;
  ushort uVar6;
  byte bVar7;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;

  candidate_65844();
  candidate_12244();
  candidate_120f8();
  *(undefined1 *)(unaff_A6 + 0x1d) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  unaff_A6[0x1c] = 0x64e02;
  *(undefined1 *)(unaff_A5 + 0x447a) = 0;
  *(undefined1 *)(unaff_A5 + 0x447b) = 0;
  candidate_40bc();
  candidate_40bc();
  candidate_40bc();
  candidate_40bc();
  candidate_40bc();
  candidate_40bc();
  *unaff_A6 = 0x65970;
  bVar8 = false;
  candidate_657d0();
  if (!bVar8) {
    candidate_ca21e();
    candidate_469c();
    candidate_8bfd6();
    candidate_1406a();
    candidate_19658();
    candidate_19e40();
    candidate_401e8();
    return;
  }
  candidate_9c0a();
  candidate_1228c();
  *(undefined1 *)(unaff_A6 + 0x1d) = 0x14;
  *unaff_A6 = 0x659ca;
  cVar5 = *(char *)(unaff_A6 + 0x1d) + -1;
  *(char *)(unaff_A6 + 0x1d) = cVar5;
  if (cVar5 != '\0') {
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  uVar3 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar3 + 0x3ef;
  if (0xfc10 < uVar3) {
    uVar3 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar4 = ~uVar3 & 0xfff;
    bVar7 = -((*(byte *)((short)(uVar3 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar4 & 7)) != 0);
    uRam002fffe0 = (ushort)(bVar7 & -(0x7ff < uVar4));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (CONCAT11(0xff,bVar7) ^ uVar6) & 1) !=
        0) {
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
