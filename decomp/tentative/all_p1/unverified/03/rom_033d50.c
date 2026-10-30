/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x033d50.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_33d50(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  ushort uVar5;
  ushort uVar6;
  undefined2 uVar7;
  ushort uVar8;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  undefined2 uVar9;
  short sVar10;
  undefined4 extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined2 *puVar11;
  undefined1 uVar12;
  bool bVar13;

  *(int *)((int)unaff_A6 + 0x7a) = *(int *)((int)unaff_A6 + 0x62) + *(int *)((int)unaff_A6 + 0x7a);
  uVar8 = *(ushort *)((int)unaff_A6 + 0x7a);
  if ((short)uVar8 < 0) {
    uVar8 = -uVar8;
  }
  uVar12 = uVar8 < *(ushort *)(unaff_A6 + 0x1c);
  if ((short)*(ushort *)(unaff_A6 + 0x1c) < (short)uVar8) {
    *(undefined4 *)((int)unaff_A6 + 0x76) = 0xc000;
    if (-1 < *(int *)((int)unaff_A6 + 0x62)) {
      *(int *)((int)unaff_A6 + 0x76) = -*(int *)((int)unaff_A6 + 0x76);
    }
    candidate_4256();
    *unaff_A6 = 0x33db4;
    uVar3 = *(uint *)((int)unaff_A6 + 0x76);
    if ((int)uVar3 < 0) {
      uVar4 = *(uint *)((int)unaff_A6 + 0x62);
      *(uint *)((int)unaff_A6 + 0x62) = uVar3 + uVar4;
      if (-1 < (int)(uVar3 + uVar4)) goto LAB_00033df0;
    }
    else {
      uVar4 = *(uint *)((int)unaff_A6 + 0x62);
      *(uint *)((int)unaff_A6 + 0x62) = uVar3 + uVar4;
      if ((int)(uVar3 + uVar4) < 0) {
LAB_00033df0:
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        candidate_4256();
        *unaff_A6 = 0x33e0a;
        if ((*(short *)(unaff_A6 + 0x1d) != 0) &&
           (sVar10 = *(short *)(unaff_A6 + 0x1d) + -1, *(short *)(unaff_A6 + 0x1d) = sVar10,
           sVar10 == 0)) {
          *(undefined2 *)(unaff_A6 + 0x1d) = 0;
          *(undefined1 *)(unaff_A6 + 0x20) = 0;
        }
        candidate_4c24();
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        if (*(char *)(unaff_A6 + 0x20) == '\0') {
          if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
            *(undefined2 *)((int)unaff_A6 + 0x7e) = 10;
            *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
            *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
            *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
            candidate_4256();
            candidate_4192();
            candidate_1dbaa();
            sVar10 = 5;
            uVar7 = 0xffd0;
            if (*(short *)((int)unaff_A6 + 0x6e) != 0) {
              uVar7 = 0xffec;
            }
            uVar9 = 0x20;
            do {
              puVar11 = (undefined2 *)register0x0000003c;
              puVar11[-2] = sVar10;
              puVar11[-4] = uVar7;
              register0x0000003c = (BADSPACEBASE *)(puVar11 + -6);
              *(undefined2 *)register0x0000003c = uVar9;
              *(undefined4 *)(puVar11 + -8) = 0x33ed4;
              candidate_411c();
              *(undefined4 *)(puVar11 + -8) = extraout_A0;
              *(undefined4 *)(puVar11 + -10) = 0x33edc;
              uVar8 = candidate_29c6();
              puVar11[-7] = uVar8 & 0x1f;
              *(undefined4 *)(puVar11 + -9) = *(undefined4 *)(puVar11 + -8);
              *(undefined4 *)(puVar11 + -0xb) = 0x33eec;
              candidate_29c6();
              uVar7 = puVar11[-5];
              puVar11[-5] = puVar11[-4];
              puVar11[-7] = uVar7;
              puVar11[-9] = *(undefined2 *)register0x0000003c;
              *(undefined4 *)(puVar11 + -0xb) = 0x33f06;
              candidate_1acf2();
              uVar9 = puVar11[-9];
              uVar7 = puVar11[-8];
              sVar10 = puVar11[-7] + -1;
            } while (sVar10 != -1);
            *(undefined4 *)(puVar11 + -8) = 0x33f18;
            candidate_1da40();
            *(undefined4 *)(puVar11 + -8) = 0x33f1e;
            candidate_19dfc();
            *(undefined4 *)(puVar11 + -8) = 0x33f28;
            candidate_120f8();
            *unaff_A6 = 0x33f2e;
            *(undefined4 *)(puVar11 + -8) = 0x33f34;
            candidate_1436a();
            *(undefined4 *)(puVar11 + -8) = 0x33f3a;
            candidate_427a();
            if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
              *(undefined4 *)(puVar11 + -8) = 0x33f4e;
              candidate_140c0();
            }
            else {
              *(undefined4 *)(puVar11 + -8) = 0x33f5c;
              candidate_140c0();
            }
            bVar13 = false;
            if (*(short *)((int)unaff_A6 + 0x72) == 0) {
              sVar10 = *(short *)((int)unaff_A6 + 0x7e);
              bVar13 = sVar10 == 0;
              sVar10 = sVar10 + -1;
              *(short *)((int)unaff_A6 + 0x7e) = sVar10;
              if (sVar10 == 0) {
                *(undefined4 *)(puVar11 + -8) = 0x33f76;
                candidate_120f8();
                *(undefined4 *)(puVar11 + -8) = 0x33f7c;
                candidate_1dbaa();
                uVar8 = *(ushort *)(unaff_A5 + 0x700c);
                *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
                if (0xfc10 < uVar8) {
                  uVar8 = *(ushort *)(unaff_A5 + 0x700a);
                  *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
                  uVar6 = uRam002fffe8;
                  uVar5 = ~uVar8 & 0xfff;
                  uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                                   -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                                     '\x01' << (uVar5 & 7)) != 0));
                  uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar5));
                  uRam002fffe2 = 0;
                  uRam002fffe4 = 0;
                  uRam002fffe6 = 0;
                  *(short *)(unaff_A5 + 0x700a) =
                       ((short)-(uVar5 == 0x400) &
                       _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
                       *(short *)(unaff_A5 + 0x700a);
                  if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (uVar8 ^ uVar6) & 1) != 0
                     ) {
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
                    *(undefined4 *)(puVar11 + -8) = 0x3d2c;
                    func_0x0003cf12();
                    *(undefined4 *)(puVar11 + -8) = 0x3d36;
                    candidate_3e2a8();
                    do {
                    /* WARNING: Do nothing block with infinite loop */
                    } while( true );
                  }
                }
                goto code_r0x00004188;
              }
            }
            *(undefined4 *)(puVar11 + -8) = 0x34014;
            candidate_1945c();
            if (!bVar13) {
              return;
            }
            goto code_r0x000041ac;
          }
        }
        else {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
          uVar7 = candidate_260c8();
          *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar7;
        }
        uVar12 = 0;
        candidate_1436a();
        candidate_427a();
        candidate_34260();
        candidate_1945c();
        if (!(bool)uVar12) {
          return;
        }
        goto code_r0x000041ac;
      }
    }
    uVar12 = CARRY4(uVar3,uVar4);
    candidate_8ccd4();
    candidate_427a();
    candidate_34260();
    candidate_1945c();
    if (!(bool)uVar12) {
      return;
    }
  }
  else {
    candidate_8ccd4();
    candidate_427a();
    candidate_34260();
    candidate_1945c();
    if (!(bool)uVar12) {
      return;
    }
  }
code_r0x000041ac:
  *(undefined4 *)((int)register0x0000003c + -4) = 0x41ae;
  candidate_4192();
code_r0x00004188:
  *(undefined4 *)((int)register0x0000003c + -4) = 0x418a;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
