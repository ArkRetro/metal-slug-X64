/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x071394.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_71394(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  ushort uVar4;
  ushort uVar5;
  undefined2 uVar7;
  uint uVar6;
  short sVar8;
  ushort uVar9;
  char cVar10;
  short extraout_D1w;
  undefined1 extraout_D1b;
  undefined4 unaff_D2;
  undefined4 unaff_D3;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar11;
  undefined1 uVar12;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7f) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x1e;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
  bVar11 = false;
  candidate_40bc();
  *(char *)(unaff_A6 + 0x18) = -!bVar11;
  candidate_1b242();
  *unaff_A6 = 0x713e4;
  uVar12 = 0;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    candidate_726da();
    candidate_7267c();
    goto LAB_00071c46;
  }
LAB_0007148e:
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
  *(undefined1 *)(unaff_A6 + 0x21) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x714b4;
  uVar12 = 0;
  uVar6 = candidate_7269c();
  if ((uVar6 & 0x80) == 0) {
    if (((((uVar6 & 2) != 0) && ((uVar6 & 0x20) != 0)) &&
        (uVar12 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) &&
       ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto code_r0x000711f8;
    if ((((uVar6 & 2) != 0) &&
        (uVar12 = *(byte *)((int)unaff_A6 + 0x87) < 0x12, *(byte *)((int)unaff_A6 + 0x87) != 0x12))
       && ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_0007153e;
    if ((((uVar6 & 2) == 0) && (uVar12 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) &&
       ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_000713fc;
    if ((uVar6 & 0x20) == 0) {
      if ((uVar6 & 8) != 0) goto LAB_00071688;
      if ((uVar6 & 4) != 0) {
LAB_000716b0:
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffe0000;
        candidate_4256();
        *(undefined1 *)((int)unaff_A6 + 0x83) = 5;
        *(undefined1 *)(unaff_A6 + 0x21) = 5;
LAB_00071728:
        do {
          while( true ) {
            *(undefined1 *)((int)unaff_A6 + 0x85) = *(undefined1 *)(unaff_A6 + 0x21);
            *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
            *unaff_A6 = 0x7173a;
            uVar6 = candidate_7269c();
            if ((uVar6 & 0x80) != 0) goto LAB_00071958;
            if (((((uVar6 & 2) != 0) && ((uVar6 & 0x20) != 0)) &&
                (*(char *)((int)unaff_A6 + 0x87) != '\0')) &&
               ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto code_r0x000711f8;
            if ((((uVar6 & 2) != 0) && (*(char *)((int)unaff_A6 + 0x87) != '\x12')) &&
               ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_0007153e;
            if ((((uVar6 & 2) == 0) && (*(char *)((int)unaff_A6 + 0x87) != '\0')) &&
               ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_000713fc;
            if ((uVar6 & 0x20) != 0) goto LAB_000715d0;
            if ((uVar6 & 8) == 0) break;
            if (0 < *(int *)((int)unaff_A6 + 0x62)) {
LAB_000717d4:
              if (*(char *)(unaff_A6 + 0x23) == '\0') {
                candidate_726da();
              }
              else {
                iVar2 = *(int *)((int)unaff_A6 + 0x62);
                if (-1 < iVar2) {
                  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
                }
                candidate_726da();
                *(int *)((int)unaff_A6 + 0x62) = iVar2;
              }
              candidate_7267c();
              *(undefined1 *)(unaff_A6 + 0x21) = *(undefined1 *)((int)unaff_A6 + 0x85);
              sVar8 = candidate_72770();
              *(short *)(unaff_A6 + 0xf) = sVar8 + *(short *)(unaff_A6 + 0xf);
              *(short *)(unaff_A6 + 0x10) = extraout_D1w + *(short *)(unaff_A6 + 0x10);
              *(short *)(unaff_A6 + 0xf) = *(short *)(unaff_A6 + 0xf) + 0x28;
              sVar8 = candidate_49e8();
              if (sVar8 != 0) {
                *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x1e;
              }
              candidate_1abc8();
              if (*(short *)((int)unaff_A6 + 0x7a) != 0) {
                *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
                *(undefined1 *)(unaff_A6 + 0x21) = 8;
              }
              uVar12 = 0;
              goto LAB_00071c46;
            }
LAB_000716d8:
            *(undefined4 *)((int)unaff_A6 + 0x62) = 0x20000;
            candidate_4256();
            *(undefined1 *)((int)unaff_A6 + 0x83) = 2;
            *(undefined1 *)(unaff_A6 + 0x21) = 2;
          }
          if ((uVar6 & 4) == 0) {
            *(char *)((int)unaff_A6 + 0x8b) = -(*(short *)((int)unaff_A6 + 0x62) < 0);
            if (*(char *)((int)unaff_A6 + 0x8b) < '\0') {
              candidate_4256();
              *(undefined1 *)((int)unaff_A6 + 0x83) = 7;
              *(undefined1 *)(unaff_A6 + 0x21) = 7;
            }
            else {
              candidate_4256();
              *(undefined1 *)((int)unaff_A6 + 0x83) = 6;
              *(undefined1 *)(unaff_A6 + 0x21) = 6;
            }
            *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
            *unaff_A6 = 0x718a4;
            uVar12 = false;
            candidate_7265a();
            if ((bool)uVar12) goto LAB_0007148e;
            uVar6 = candidate_7269c();
            if ((uVar6 & 0x80) != 0) goto LAB_00071958;
            if (((((uVar6 & 2) != 0) && ((uVar6 & 0x20) != 0)) &&
                (uVar12 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) &&
               ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto code_r0x000711f8;
            if ((((uVar6 & 2) != 0) &&
                (uVar12 = *(byte *)((int)unaff_A6 + 0x87) < 0x12,
                *(byte *)((int)unaff_A6 + 0x87) != 0x12)) &&
               ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_0007153e;
            if ((((uVar6 & 2) == 0) && (uVar12 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) &&
               ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto LAB_000713fc;
            if ((uVar6 & 0x20) != 0) goto LAB_000715d0;
            if ((uVar6 & 8) == 0) {
              if ((uVar6 & 4) == 0) {
                candidate_726da();
                candidate_7267c();
                if ((bool)uVar12) {
                  *unaff_A6 = 0x7148e;
                  uVar12 = 0;
                }
                goto LAB_00071c46;
              }
              if (-1 < *(char *)((int)unaff_A6 + 0x8b)) goto LAB_000716d8;
              goto LAB_000716b0;
            }
            if (-1 < *(char *)((int)unaff_A6 + 0x8b)) {
LAB_00071688:
              *(undefined4 *)((int)unaff_A6 + 0x62) = 0x20000;
              candidate_4256();
              *(undefined1 *)((int)unaff_A6 + 0x83) = 4;
              *(undefined1 *)(unaff_A6 + 0x21) = 4;
              goto LAB_00071728;
            }
          }
          else if (*(int *)((int)unaff_A6 + 0x62) < 0) goto LAB_000717d4;
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfffe0000;
          candidate_4256();
          *(undefined1 *)((int)unaff_A6 + 0x83) = 3;
          *(undefined1 *)(unaff_A6 + 0x21) = 3;
        } while( true );
      }
      candidate_726da();
      candidate_7267c();
      goto LAB_00071c46;
    }
LAB_000715d0:
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    if (*(char *)(unaff_A6 + 0x22) == '\0') goto LAB_000715e0;
    goto LAB_00071634;
  }
LAB_00071958:
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 4;
  *(undefined1 *)((int)unaff_A6 + 0x83) = 1;
  if (*(char *)((int)unaff_A6 + 0x87) != '\x12') {
    candidate_4256();
    *unaff_A6 = 0x71a20;
    uVar12 = 0;
    candidate_726da();
    candidate_7267c();
    if ((bool)uVar12) {
      *unaff_A6 = 0x71972;
    }
    *(undefined2 *)(unaff_A6 + 0x1e) = 0x8000;
    uVar12 = 0;
    goto LAB_00071c46;
  }
  if (*(char *)(unaff_A6 + 0x22) != '\0') {
    if (*(char *)((int)unaff_A6 + 0x87) != '\x12') goto LAB_00071304;
    candidate_4256();
    *unaff_A6 = 0x719ee;
    uVar12 = 0;
    candidate_726da();
    candidate_7267c();
    if ((bool)uVar12) {
      *unaff_A6 = 0x71972;
    }
    *(undefined2 *)(unaff_A6 + 0x1e) = 0x8000;
    uVar12 = 0;
    goto LAB_00071c46;
  }
  bVar11 = false;
  candidate_40bc();
  *(char *)(unaff_A6 + 0x18) = -!bVar11;
  candidate_1b242();
  candidate_4256();
  *unaff_A6 = 0x719b4;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    candidate_726da();
    candidate_7267c();
    *(undefined2 *)(unaff_A6 + 0x1e) = 0x8000;
    uVar12 = 0;
    goto LAB_00071c46;
  }
  candidate_aa7fc();
  candidate_4256();
  *(undefined1 *)(unaff_A6 + 0x21) = 2;
  *unaff_A6 = 0x71a66;
  if (*(int *)((int)unaff_A6 + 0x6e) != 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_72716();
  }
  uVar9 = candidate_49e8();
  if (5 < uVar9) goto LAB_00071aac;
                    /* WARNING: Switch is manually overridden */
  switch(uVar9 << 2) {
  case 0:
    break;
  case 4:
    break;
  case 8:
    goto LAB_00071ae2;
  case 0xc:
    goto LAB_00071ae2;
  case 0x10:
    break;
  case 0x14:
  }
LAB_00071aac:
  if ((0x110 < *(short *)(unaff_A6 + 0xf)) || ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0)) {
LAB_00071ae2:
    candidate_49e8();
    candidate_ae220();
    candidate_ad9e6();
    *(undefined4 *)(extraout_A0 + 0x30) = 0xfff30000;
    *(undefined4 *)(extraout_A0 + 0x34) = 0x180000;
    *(undefined2 *)(extraout_A0 + 0x4e) = 10;
    candidate_ad9e6();
    *(undefined4 *)(extraout_A0_00 + 0x30) = 0xb0000;
    *(undefined4 *)(extraout_A0_00 + 0x34) = 0x150000;
    *(undefined2 *)(extraout_A0_00 + 0x4e) = 7;
    candidate_ad9e6();
    *(undefined4 *)(extraout_A0_01 + 0x30) = 0x80000;
    *(undefined4 *)(extraout_A0_01 + 0x34) = 0xfffc0000;
    *(undefined2 *)(extraout_A0_01 + 0x4e) = 5;
    candidate_411c();
    candidate_1acf2();
    uVar9 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
    if (0xfc10 < uVar9) {
      uVar9 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar5 = uRam002fffe8;
      uVar4 = ~uVar9 & 0xfff;
      uVar9 = (ushort)CONCAT31((int3)((uint)unaff_D2 >> 8),
                               -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar4 & 7)) != 0));
      uRam002fffe0 = uVar9 & (ushort)CONCAT31((int3)((uint)unaff_D3 >> 8),-(0x7ff < uVar4));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar9 ^ uVar5) & 1) != 0) {
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
  *(int *)((int)unaff_A6 + 0x62) =
       (0xc0000 - *(int *)((int)unaff_A6 + 0x62) >> 5) + *(int *)((int)unaff_A6 + 0x62);
  candidate_726b8();
  candidate_7267c();
  *(undefined1 *)((int)unaff_A6 + 0x83) = 0xff;
  goto LAB_00071d66;
  while( true ) {
    if (((((uVar6 & 2) != 0) && ((uVar6 & 0x20) != 0)) &&
        (uVar12 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) &&
       ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) goto code_r0x000711f8;
    if ((((uVar6 & 2) == 0) ||
        (uVar12 = *(byte *)((int)unaff_A6 + 0x87) < 0x12, *(byte *)((int)unaff_A6 + 0x87) == 0x12))
       || ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
      candidate_726da();
      candidate_7267c();
      if ((bool)uVar12) {
        *unaff_A6 = 0x7148e;
        uVar12 = 0;
      }
      goto LAB_00071c46;
    }
LAB_0007153e:
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x83) = 1;
    *(undefined1 *)(unaff_A6 + 0x21) = 1;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
    *unaff_A6 = 0x71570;
    uVar12 = 0;
    uVar6 = candidate_7269c();
    if ((uVar6 & 0x80) != 0) goto LAB_00071958;
    if ((((uVar6 & 2) != 0) && ((uVar6 & 0x20) != 0)) &&
       ((uVar12 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0' &&
        ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)))) goto code_r0x000711f8;
    if ((((uVar6 & 2) != 0) || (uVar12 = 0, *(char *)((int)unaff_A6 + 0x87) == '\0')) ||
       ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) break;
LAB_000713fc:
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x83) = 1;
    *(undefined1 *)(unaff_A6 + 0x21) = 1;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
    *unaff_A6 = 0x7142e;
    uVar12 = 0;
    uVar6 = candidate_7269c();
    if ((uVar6 & 0x80) != 0) goto LAB_00071958;
  }
  candidate_726da();
  candidate_7267c();
  if ((bool)uVar12) {
    *unaff_A6 = 0x7148e;
    uVar12 = 0;
  }
  goto LAB_00071c46;
code_r0x000711f8:
  *(undefined1 *)((int)unaff_A6 + 0x83) = 0xff;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
  candidate_1b1dc();
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  if (*(char *)((int)unaff_A6 + 0x87) == '\x12') {
    if (*(char *)(unaff_A6 + 0x22) == '\0') {
      candidate_353a4();
      candidate_4256();
      *(undefined1 *)(unaff_A6 + 0x21) = 1;
      *unaff_A6 = 0x7124a;
      uVar12 = 0;
      candidate_726b8();
      candidate_7267c();
      candidate_1945c();
      if ((bool)uVar12) {
        *unaff_A6 = candidate_71d6e;
        uVar12 = 0;
      }
      uVar7 = candidate_aa610();
      if ((bool)uVar12) {
        *(undefined2 *)((int)unaff_A6 + 0x72) = uVar7;
        bVar11 = false;
        uVar7 = candidate_aa760();
        if (bVar11) {
          *(undefined2 *)(unaff_A6 + 0x1d) = uVar7;
          candidate_aa83a();
          *unaff_A6 = candidate_71394;
        }
      }
      goto LAB_00071d66;
    }
    if (*(char *)((int)unaff_A6 + 0x87) == '\x12') {
      candidate_4256();
      *(undefined1 *)(unaff_A6 + 0x21) = 1;
      *unaff_A6 = 0x712d8;
      uVar12 = 0;
      candidate_726b8();
      candidate_7267c();
      if ((bool)uVar12) {
        *unaff_A6 = 0x71212;
        uVar12 = 0;
      }
      candidate_1945c();
      if ((bool)uVar12) {
        *unaff_A6 = candidate_71d6e;
      }
      goto LAB_00071d66;
    }
  }
LAB_00071304:
  candidate_4256();
  *(undefined1 *)(unaff_A6 + 0x21) = 1;
  *unaff_A6 = 0x71320;
  uVar12 = 0;
  candidate_726b8();
  candidate_7267c();
  if ((bool)uVar12) {
    *unaff_A6 = 0x71212;
    uVar12 = 0;
  }
  candidate_1945c();
  if ((bool)uVar12) {
    *unaff_A6 = candidate_71d6e;
  }
  goto LAB_00071d66;
LAB_000715e0:
  while( true ) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x83) = 1;
    *(undefined1 *)(unaff_A6 + 0x21) = 1;
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 1;
    *unaff_A6 = 0x71608;
    uVar12 = 0;
    uVar6 = candidate_7269c();
    bVar3 = *(byte *)(unaff_A6 + 0x1f);
    *(byte *)(unaff_A6 + 0x1f) = bVar3 & 0xfe;
    if (((bVar3 & 1) != 0) || ((uVar6 & 0x20) == 0)) break;
LAB_00071634:
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x83) = 1;
    *(undefined1 *)(unaff_A6 + 0x21) = 1;
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 1;
    *unaff_A6 = 0x7165c;
    uVar12 = 0;
    uVar6 = candidate_7269c();
    bVar3 = *(byte *)(unaff_A6 + 0x1f);
    *(byte *)(unaff_A6 + 0x1f) = bVar3 & 0xfe;
    if (((bVar3 & 1) != 0) || ((uVar6 & 0x20) == 0)) {
      candidate_726da();
      candidate_7267c();
      if ((bool)uVar12) {
        *unaff_A6 = 0x7148e;
        uVar12 = 0;
      }
      goto LAB_00071c46;
    }
  }
  candidate_726da();
  candidate_7267c();
  if ((bool)uVar12) {
    *unaff_A6 = 0x7148e;
    uVar12 = 0;
  }
LAB_00071c46:
  candidate_727c0();
  candidate_72784();
  if ((bool)uVar12) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 2;
  }
  if (*(short *)((int)unaff_A6 + 0x76) != 0) {
    *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar3 = *(byte *)((int)unaff_A6 + 0x5f);
  *(byte *)((int)unaff_A6 + 0x5f) = bVar3 & 0xbf;
  if ((bVar3 & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if (*(int *)((int)unaff_A6 + 0x6e) == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
    if ((*(ushort *)((int)unaff_A6 + 0x76) & 1) != 0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_72716();
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((DAT_0010f000 & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  uVar12 = candidate_72770();
  bVar11 = false;
  candidate_aa760();
  if (bVar11) {
    cVar10 = candidate_aaa88();
    if (cVar10 == '\n') {
      *(undefined1 *)(unaff_A6 + 0x20) = extraout_D1b;
      *(undefined1 *)((int)unaff_A6 + 0x81) = uVar12;
    }
  }
  else {
    *unaff_A6 = candidate_711f8;
  }
  if (*(char *)((int)unaff_A6 + 0x7d) != '\0') {
    *(char *)((int)unaff_A6 + 0x7d) = *(char *)((int)unaff_A6 + 0x7d) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x7e) != '\0') {
    *(char *)((int)unaff_A6 + 0x7e) = *(char *)((int)unaff_A6 + 0x7e) + -1;
  }
  if (*(char *)(unaff_A6 + 0x23) != '\0') {
    *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x7f) != '\0') {
    *(char *)((int)unaff_A6 + 0x7f) = *(char *)((int)unaff_A6 + 0x7f) + -1;
  }
  bVar11 = false;
  candidate_aa9e4();
  if (bVar11) {
    *(undefined2 *)((int)unaff_A6 + 0x76) = 0x1e;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
LAB_00071d66:
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
