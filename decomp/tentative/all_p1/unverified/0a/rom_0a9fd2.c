/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a9fd2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_a9fd2(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  byte bVar4;
  ushort uVar5;
  ushort uVar6;
  short sVar7;
  undefined2 unaff_D2w;
  undefined4 unaff_D3;
  byte bVar8;
  int extraout_A0;
  int extraout_A0_00;
  short *psVar9;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;

  *(short *)(unaff_A6 + 0x1d) =
       (short)((uint)*(ushort *)((short)(*(short *)(unaff_A6 + 0x1c) * 2) + 0x2d1b0c) *
               (uint)*(ushort *)((int)unaff_A6 + 0x72) >> 8);
  if (*(int *)((short)(*(short *)(unaff_A6 + 0x1c) * 4) + 0xe6836) != 0) {
    candidate_4256();
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + 1;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *unaff_A6 = 0xaa024;
    candidate_1436a();
    candidate_4c24();
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar10 = 0;
    candidate_1ad8c();
    candidate_427a();
    if (((bool)uVar10) &&
       (uVar10 = *(ushort *)((int)unaff_A6 + 0x4e) < *(ushort *)(unaff_A6 + 0x1d),
       (short)*(ushort *)((int)unaff_A6 + 0x4e) <= (short)*(ushort *)(unaff_A6 + 0x1d))) {
      candidate_120f8();
      *unaff_A6 = candidate_a9fd2;
      if ((*(byte *)(*(short *)(unaff_A6 + 0x1c) + 0xe653a) & 1) != 0) {
        candidate_1da40();
        *(undefined2 *)(extraout_A0 + 0x66) = 0x1001;
        *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
      }
      uVar10 = 0;
      if ((*(byte *)(*(short *)(unaff_A6 + 0x1c) + 0xe653a) & 2) != 0) {
        candidate_1da40();
        *(undefined2 *)(extraout_A0_00 + 0x66) = 0xe0ed;
        *(undefined2 *)(extraout_A0_00 + 0x1e) = 0xe0ed;
        uVar10 = 0;
      }
    }
    candidate_1945c();
    if ((bool)uVar10) {
      uVar6 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
      if (0xfc10 < uVar6) {
        uVar6 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar5 = uRam002fffe8;
        uVar3 = ~uVar6 & 0xfff;
        uVar6 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)
                           ) != 0));
        uRam002fffe0 = uVar6 & (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar3));
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
    candidate_140c0();
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) && (*(char *)((int)unaff_A6 + 0x7a) == '\0')
       ) {
      return;
    }
  }
  *(undefined2 *)(unaff_A6 + 0x1e) = 0xffd0;
  uVar6 = candidate_29c6();
  *(undefined1 *)((int)unaff_A6 + 0x7b) = *(undefined1 *)((short)(uVar6 & 0x1f) + 0xe6516);
  sVar7 = candidate_29f2();
  psVar9 = *(short **)((short)(sVar7 * 4) + 0x2d1aac);
  sVar7 = 3;
  do {
    if (-1 < *psVar9) {
      bVar8 = *(byte *)((int)unaff_A6 + 0x7b);
      do {
        *(short *)(unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x1e) + 0x10;
        bVar4 = bVar8 & 1;
        bVar8 = bVar8 >> 1;
      } while (bVar4 == 0);
      *(byte *)((int)unaff_A6 + 0x7b) = bVar8;
      candidate_8cdb2();
    }
    psVar9 = psVar9 + 1;
    sVar7 = sVar7 + -1;
  } while (sVar7 != -1);
  candidate_aa28e();
  candidate_1da40();
  *unaff_A6 = candidate_aa1fe;
  return;
}
