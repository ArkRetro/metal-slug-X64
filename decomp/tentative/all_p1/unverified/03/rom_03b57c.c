/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03b57c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_3b57c(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  char cVar5;
  uint uVar6;
  short sVar7;
  undefined4 unaff_D2;
  short sVar8;
  char cVar10;
  ushort uVar9;
  undefined2 unaff_D3w;
  char unaff_D6b;
  int extraout_A0;
  ushort *extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar11;
  int extraout_A0_03;
  undefined4 extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar12;
  undefined1 in_CF;
  undefined1 uVar13;
  undefined8 uVar14;
  undefined4 uStack_4;

  uStack_4 = 0x3b582;
  candidate_409a();
  if (((!(bool)in_CF) || ((*(byte *)(extraout_A0 + 0x5f) & 0x80) != 0)) ||
     (**(short **)((int)unaff_A6 + 0x6e) == -0x8000)) {
    uStack_4 = 0x3b874;
    cVar5 = candidate_aab84();
    if (-1 < cVar5) {
      uStack_4 = 0x3b882;
      candidate_aabdc();
    }
    uStack_4 = 0x3b898;
    candidate_411c();
    *(undefined1 *)(extraout_A0_03 + 0x72) = *(undefined1 *)((int)unaff_A6 + 0x72);
    uStack_4 = 0x3b8a8;
    candidate_1b220();
    uVar9 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
    if (0xfc10 < uVar9) {
      uVar9 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar9 & 0xfff;
      uVar9 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                               -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar2 & 7)) != 0));
      uRam002fffe0 = uVar9 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar2));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar9 ^ uVar3) & 1) != 0) {
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
        uStack_4 = 0x3d2c;
        func_0x0003cf12();
        uStack_4 = 0x3d36;
        candidate_3e2a8();
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
    }
    uStack_4 = 0x418a;
    candidate_4126();
    iVar11 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar11 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if (**(short **)((int)unaff_A6 + 0x6e) == -0x7f80) {
    return;
  }
  *(ushort *)(unaff_A6 + 0x21) = *(ushort *)(unaff_A6 + 0x21) >> 1;
  uStack_4._2_2_ = 0xb5b2;
  uStack_4._0_2_ = 3;
  bVar4 = candidate_aab84();
  if ((char)bVar4 < '\0') {
    *(undefined1 *)(unaff_A6 + 0x1e) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x76) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x77) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x89) = 0;
    goto LAB_0003b7fa;
  }
  *(ushort *)((int)unaff_A6 + 0x86) = (ushort)bVar4;
  *(char *)((int)unaff_A6 + 0x5e) = (char)(1 << ((ushort)bVar4 & 0x1f));
  uStack_4 = 0x3b5f4;
  candidate_aabdc();
  uStack_4._2_2_ = 0xb5fa;
  uStack_4._0_2_ = 3;
  candidate_ca2ce();
  uVar9 = *extraout_A0_00;
  if ((uVar9 & 0x10) == 0) {
    uVar9 = extraout_A0_00[1];
    if ((uVar9 & 0x10) == 0) goto LAB_0003b65c;
    bVar12 = *(short *)((int)unaff_A6 + 0x86) == 0;
    uStack_4._0_2_ = uVar9;
    candidate_ca614();
    register0x0000003c = (BADSPACEBASE *)((int)&uStack_4 + 2);
    uVar9 = uStack_4._0_2_;
    if ((bVar12) ||
       (cVar5 = *(char *)((int)unaff_A6 + 0x89), *(char *)((int)unaff_A6 + 0x89) = cVar5 + -1,
       cVar5 != '\0')) goto LAB_0003b65c;
    *(undefined1 *)((int)unaff_A6 + 0x89) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x77) = *(undefined1 *)((int)unaff_A6 + 0x75);
LAB_0003b682:
    uVar6 = (uint)*(ushort *)(unaff_A6 + 0x1f);
    *(ushort *)((int)unaff_A6 + 0x82) = *(ushort *)(unaff_A6 + 0x1f);
  }
  else {
    if (*(char *)((int)unaff_A6 + 0x77) == '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x76) = *(undefined1 *)(unaff_A6 + 0x1d);
    }
    *(undefined1 *)((int)unaff_A6 + 0x77) = *(undefined1 *)((int)unaff_A6 + 0x75);
    *(undefined1 *)((int)unaff_A6 + 0x89) = 8;
LAB_0003b65c:
    uVar6 = *(uint *)(*(int *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x72) << 2) + 0x3b4b0) +
                     (int)(short)((uVar9 & 0xf) * 4));
    if ((int)uVar6 < 0) goto LAB_0003b682;
  }
  sVar8 = *(short *)((int)unaff_A6 + 0x82);
  sVar7 = (short)uVar6;
  *(short *)((int)unaff_A6 + 0x82) = sVar7;
  if (sVar7 != sVar8) {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x800;
    *(undefined2 *)(unaff_A6 + 0x20) = 0;
  }
  uVar9 = *(short *)(unaff_A6 + 0x20) + *(short *)((int)unaff_A6 + 0x7e);
  sVar7 = sVar7 - *(short *)(unaff_A6 + 0x1f);
  if (sVar7 != 0) {
    if (sVar7 == -0x8000) {
      *(byte *)((int)unaff_A6 + 0x79) = ~*(byte *)((int)unaff_A6 + 0x79);
    }
    else {
      *(char *)((int)unaff_A6 + 0x79) = -(-1 < (short)(sVar7 + -0x8000));
    }
    if (*(char *)((int)unaff_A6 + 0x79) != '\0') {
      uVar9 = -uVar9;
    }
    sVar8 = uVar9 + *(short *)(unaff_A6 + 0x1f);
    if ((short)(sVar7 - uVar9 ^ uVar9) < 0) {
      sVar8 = (sVar7 - uVar9) + sVar8;
    }
    *(short *)(unaff_A6 + 0x1f) = sVar8;
    *(short *)((int)unaff_A6 + 0x7e) =
         ((short)(-0x600 - *(short *)((int)unaff_A6 + 0x7e)) >> 2) +
         *(short *)((int)unaff_A6 + 0x7e);
    *(short *)(unaff_A6 + 0x20) =
         ((short)(0x3000 - *(short *)(unaff_A6 + 0x20)) >> 4) + *(short *)(unaff_A6 + 0x20);
  }
  if (*(char *)((int)unaff_A6 + 0x77) == '\0') {
    *(undefined1 *)(unaff_A6 + 0x1e) = 0;
  }
  else {
    cVar5 = *(char *)((int)unaff_A6 + 0x76);
    *(char *)((int)unaff_A6 + 0x76) = cVar5 + -1;
    if (cVar5 == '\0') {
      *(char *)((int)unaff_A6 + 0x77) = *(char *)((int)unaff_A6 + 0x77) + -1;
      *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
      *(char *)((int)unaff_A6 + 0x76) =
           *(char *)((int)unaff_A6 + 0x73) + *(char *)((int)unaff_A6 + 0x76);
      if (*(char *)((int)unaff_A6 + 0x72) == '\x05') {
        *(undefined4 *)((int)register0x0000003c + -4) = 0x3b768;
        candidate_411c();
        iVar11 = extraout_A0_02;
      }
      else {
        *(undefined4 *)((int)register0x0000003c + -4) = 0x3b74e;
        candidate_411c();
        iVar11 = extraout_A0_01;
      }
      *(undefined1 *)(iVar11 + 0x5e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
      cVar10 = '\0';
      cVar5 = *(char *)(unaff_A6 + 0x1f);
      if (cVar5 == *(char *)((int)unaff_A6 + 0x7b)) {
        bVar4 = *(char *)((int)unaff_A6 + 0x7a) + 1;
        if (2 < bVar4) {
          bVar4 = 0;
        }
        *(byte *)((int)unaff_A6 + 0x7a) = bVar4;
        cVar10 = (bVar4 - 1) * '\x02';
      }
      *(char *)((int)unaff_A6 + 0x7b) = cVar5;
      *(char *)(iVar11 + 0x6e) = cVar10 + cVar5;
      *(int *)((int)register0x0000003c + -4) = iVar11;
      *(undefined4 *)((int)register0x0000003c + -8) = 0x3b7ae;
      candidate_2e7c();
      *(undefined4 *)((int)register0x0000003c + -4) = 0x3b7c0;
      uVar14 = candidate_14a02();
      if ((unaff_D6b != '\0') && (*(short *)(unaff_A6 + 0x21) == 0)) {
        *(undefined2 *)(unaff_A6 + 0x21) = 0xff;
        *(undefined4 *)((int)register0x0000003c + -4) = extraout_A1;
        *(undefined8 *)((int)register0x0000003c + -0xc) = uVar14;
        *(undefined4 *)((int)register0x0000003c + -0x10) = 0x3b7e8;
        candidate_40bc();
        *(undefined4 *)((int)register0x0000003c + -4) = 0x3b7fa;
        candidate_1ab98();
      }
    }
  }
LAB_0003b7fa:
  *(byte *)(unaff_A6 + 0x22) =
       ((char)*(byte *)(unaff_A6 + 0x1f) >> 3) + ((*(byte *)(unaff_A6 + 0x1f) >> 2 & 1) != 0) & 0x1f
  ;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  cVar5 = (*(char **)((int)unaff_A6 + 0x6e))[1];
  *(short *)(unaff_A6 + 10) = (short)**(char **)((int)unaff_A6 + 0x6e);
  *(short *)(unaff_A6 + 0xb) = (short)cVar5;
  uVar13 = 0;
  *(undefined4 *)((int)register0x0000003c + -4) = 0x3b834;
  candidate_1abc8();
  *(undefined4 *)((int)register0x0000003c + -4) = 0x3b83a;
  candidate_44f6();
  if ((bool)uVar13) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  *(undefined4 *)((int)register0x0000003c + -4) = 0x3b850;
  candidate_427a();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  if (*(char *)(*(int *)((int)unaff_A6 + 0x6e) + 2) != '\0') {
    *(undefined1 *)(*(int *)((int)unaff_A6 + 0x6e) + 2) = 0;
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  return;
}
