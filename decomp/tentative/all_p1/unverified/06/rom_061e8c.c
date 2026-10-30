/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x061e8c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_61e8c(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  uint uVar4;
  ushort uVar5;
  int iVar6;
  ushort uVar7;
  short sVar9;
  int iVar8;
  undefined2 uVar10;
  undefined2 unaff_D2w;
  ushort uVar11;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar12;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  unaff_A6[0x14] = 0xe1246;
  candidate_1aca2();
  candidate_1ad6e();
  candidate_4256();
  sVar9 = candidate_260c8();
  iVar8 = sVar9 * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar8 = sVar9 * -0x100;
  }
  unaff_A6[0xc] = iVar8;
  unaff_A6[0xd] = 0;
  uVar10 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar10;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0x61f26;
  if (-1 < 0x6000 - unaff_A6[0xd]) {
    unaff_A6[0xd] = (0x6000 - unaff_A6[0xd] >> 3) + unaff_A6[0xd];
  }
  func_0x000143e4();
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    iVar8 = unaff_A6[0xc];
    if (iVar8 < 0) {
      iVar8 = -iVar8;
    }
    iVar2 = *(int *)((int)unaff_A6 + 0x76);
    iVar6 = iVar2 - iVar8;
    *(int *)((int)unaff_A6 + 0x76) = iVar6;
    *(char *)((int)unaff_A6 + 0x61) = -(iVar6 == 0 || iVar2 < iVar8);
  }
  if (((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) || (uVar12 = 0, unaff_A6[0xd] == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    *unaff_A6 = 0x61f84;
    uVar12 = 0;
    candidate_1abc8();
  }
  candidate_427a();
  if ((bool)uVar12) {
    candidate_4256();
    *unaff_A6 = 0x61fc2;
    uVar12 = 0;
    candidate_1ac38();
    candidate_427a();
    if ((bool)uVar12) goto LAB_0006201c;
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
    uVar12 = false;
    if (*(char *)((int)unaff_A6 + 0x61) < '\0') {
      uVar4 = unaff_A6[0xd];
      uVar12 = 0xffffefff < uVar4;
      unaff_A6[0xd] = uVar4 + 0x1000;
    }
  }
  else {
    sVar9 = candidate_49e8();
    uVar12 = false;
    if (sVar9 != 0) goto LAB_0006201c;
  }
  candidate_4c24();
  if (((!(bool)uVar12) ||
      ((bVar3 = *(byte *)((int)unaff_A6 + 0x5a), bVar3 != 10 && (uVar12 = bVar3 < 0xb, bVar3 != 0xb)
       ))) && (candidate_19504(), !(bool)uVar12)) {
    return;
  }
LAB_0006201c:
  uVar11 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar11 + 0x3ef;
  if (0xfc10 < uVar11) {
    uVar11 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar7 = uRam002fffe8;
    uVar5 = ~uVar11 & 0xfff;
    uVar11 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                      -((*(byte *)((short)(uVar11 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7))
                       != 0));
    uRam002fffe0 = uVar11 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar11 ^ uVar7) & 1) != 0) {
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
  iVar8 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar8 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
