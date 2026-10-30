/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x034786.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_34786(void)

{
  code *pcVar1;
  ushort uVar2;
  short sVar3;
  ushort uVar4;
  undefined2 uVar7;
  uint uVar5;
  int iVar6;
  undefined3 uVar8;
  undefined4 unaff_D2;
  ushort uVar9;
  undefined3 uVar10;
  undefined4 unaff_D3;
  uint unaff_D7;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar11;
  undefined1 uVar12;

  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x80;
  candidate_350e4();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x3479c;
  candidate_ca2ce();
  bVar11 = false;
  candidate_3517c();
  if (!bVar11) {
    uVar5 = candidate_35168();
    if (!bVar11) {
      if ((uVar5 & 2) == 0) {
        uVar9 = *(ushort *)((int)unaff_A6 + 0x7e);
        *(ushort *)((int)unaff_A6 + 0x7e) = uVar9 - 0x80;
        if (uVar9 < 0x80) {
          *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
          *unaff_A6 = candidate_346ea;
        }
      }
      else {
        *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + 0x80;
        *(ushort *)((int)unaff_A6 + 0x7e) = *(ushort *)((int)unaff_A6 + 0x7e) & 0x3ff;
        if (0x37f < *(ushort *)((int)unaff_A6 + 0x7e)) {
          *unaff_A6 = candidate_34852;
        }
      }
      if ((uVar5 & 0x20) != 0) {
        *unaff_A6 = candidate_349be;
      }
      candidate_35100();
      candidate_3546a();
      if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) || ((short)(unaff_D7 >> 0x10) != 5)) {
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          *unaff_A6 = 0x349c6;
        }
        sVar3 = (ushort)*(byte *)((int)unaff_A6 + 0x7e) * 2;
        *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(sVar3 + 0x34340);
        *(undefined1 *)((int)unaff_A6 + 0x79) = *(undefined1 *)(sVar3 + 0x34341);
        candidate_427a();
      }
      else {
        candidate_3c532();
        *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
        candidate_350e4();
        unaff_A6[0xc] = 0;
        unaff_A6[0xd] = 0;
        *(undefined2 *)(unaff_A6 + 0x1f) = 0;
        *(undefined1 *)(unaff_A6 + 0x23) = 0;
        *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
        uVar5 = candidate_29c6();
        *(char *)((int)unaff_A6 + 0x8b) = -((uVar5 & 3) == 0);
        *unaff_A6 = 0x34afa;
        if (*(char *)((int)unaff_A6 + 0x87) != '\0') {
          *(undefined1 *)((int)unaff_A6 + 0x89) = 0xff;
          candidate_aa760();
          candidate_aa7fc();
          *unaff_A6 = 0x34b28;
        }
        candidate_35486();
        candidate_427a();
        if (*(char *)(unaff_A6 + 0x23) != '\0') {
          uVar9 = *(short *)(unaff_A6 + 0x1f) + 1;
          *(ushort *)(unaff_A6 + 0x1f) = uVar9;
          if (0x1e < uVar9) {
            *unaff_A6 = 0x34aca;
          }
          if ((9 < uVar9) && (uVar5 = candidate_29c6(), (uVar5 & 0xf) == 0)) {
            *unaff_A6 = 0x34aca;
          }
        }
        if ((*(char *)((int)unaff_A6 + 0x8e) == '\0') && (*(char *)((int)unaff_A6 + 0x8d) != '\0'))
        {
          *(undefined1 *)((int)unaff_A6 + 0x8e) = 0xff;
          candidate_40bc();
          candidate_1acf2();
        }
      }
      if (*(char *)((int)unaff_A6 + 0x87) == '\0') {
        unaff_A6[0x14] =
             *(undefined4 *)((short)(*(byte *)((int)unaff_A6 + 0x86) - 0x29 & 0x1c) + 0x342b8);
        if (*(short *)(unaff_A6 + 0x20) == 0) {
          candidate_aa9e4();
          if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
          }
          *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        }
        else {
          *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
          candidate_aa9e4();
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
          *(undefined2 *)((int)unaff_A6 + 0x4e) = 0;
          *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        }
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
          candidate_354da();
        }
        bVar11 = false;
        candidate_aa760();
        if (!bVar11) {
          *unaff_A6 = 0x345a0;
        }
        *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + 1;
      }
      return;
    }
    candidate_aa7fc();
    *(undefined1 *)((int)unaff_A6 + 0x89) = 0xff;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    *(undefined1 *)(unaff_A6 + 0x24) = 1;
LAB_00034bbe:
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *(undefined2 *)(unaff_A6 + 0x21) = 0xf;
    *unaff_A6 = candidate_34bf0;
    iVar6 = 0x60000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      iVar6 = -0x60000;
    }
    *(int *)((int)unaff_A6 + 0x62) =
         (iVar6 - *(int *)((int)unaff_A6 + 0x62) >> 3) + *(int *)((int)unaff_A6 + 0x62);
    candidate_35496();
    *(char *)((int)unaff_A6 + 0x8f) = -((short)(unaff_D7 >> 0x10) == 5);
    candidate_49e8();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 2) == 0 && (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xf7;
      *(undefined2 *)(unaff_A6 + 0x21) = 0xf;
    }
    else {
      uVar8 = (undefined3)((uint)unaff_D2 >> 8);
      uVar10 = (undefined3)((uint)unaff_D3 >> 8);
      if (*(char *)(unaff_A6 + 0x24) != '\0') {
        *(char *)(unaff_A6 + 0x24) = *(char *)(unaff_A6 + 0x24) + -1;
        *(undefined2 *)(unaff_A6 + 0x21) = 0xc;
        *(undefined1 *)((int)unaff_A6 + 0x12) = 0xc0;
        candidate_49e8();
        *unaff_A6 = 0x34d58;
        uVar9 = *(ushort *)(unaff_A6 + 0x21) - 1;
        *(ushort *)(unaff_A6 + 0x21) = uVar9;
        if (-1 < (int)((uint)uVar9 << 0x10)) goto code_r0x00034d60;
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
        unaff_D7 = unaff_D7 << 0x10 | unaff_D7 >> 0x10;
        goto LAB_00034bbe;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) != 0) {
        uVar9 = *(ushort *)(unaff_A6 + 0x21) - 1;
        *(ushort *)(unaff_A6 + 0x21) = uVar9;
        if (-1 < (int)((uint)uVar9 << 0x10)) goto LAB_00034d0a;
        uVar9 = *(ushort *)(unaff_A5 + 0x700c);
        *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
        if (uVar9 < 0xfc11) goto candidate_4188;
        uVar9 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar4 = uRam002fffe8;
        uVar2 = ~uVar9 & 0xfff;
        uVar9 = (ushort)CONCAT31(uVar8,-((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                                         '\x01' << (uVar2 & 7)) != 0));
        uRam002fffe0 = uVar9 & (ushort)CONCAT31(uVar10,-(0x7ff < uVar2));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        uVar9 = (ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar9 ^ uVar4);
        goto joined_r0x00034e06;
      }
      uVar9 = *(ushort *)(unaff_A6 + 0x21) - 1;
      *(ushort *)(unaff_A6 + 0x21) = uVar9;
      if ((int)((uint)uVar9 << 0x10) < 0) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
        *(undefined2 *)(unaff_A6 + 0x21) = 0x1e;
      }
    }
LAB_00034d0a:
    uVar5 = *(uint *)((int)unaff_A6 + 0x62);
    if ((int)uVar5 < 0) {
      uVar5 = -uVar5;
    }
    *(short *)((int)unaff_A6 + 0x26) = (short)(uVar5 >> 9);
    uVar12 = 0;
    candidate_427a();
    candidate_1945c();
    if (!(bool)uVar12) {
      return;
    }
    goto candidate_4188;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x89) = 0xff;
  candidate_9f88();
  candidate_9ea8();
  candidate_350e4();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0;
  candidate_353a4();
  *unaff_A6 = 0x345e0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
    candidate_143ac();
  }
  else {
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + 0x80;
    if (0x380 < *(short *)((int)unaff_A6 + 0x7e)) {
      *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x380;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    candidate_14574();
  }
  sVar3 = (ushort)*(byte *)((int)unaff_A6 + 0x7e) * 2;
  *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)(sVar3 + 0x34340);
  *(undefined1 *)((int)unaff_A6 + 0x79) = *(undefined1 *)(sVar3 + 0x34341);
  uVar12 = 0;
  candidate_427a();
  candidate_c8bec();
  if (!(bool)uVar12) {
    uVar7 = candidate_aa610();
    if ((bool)uVar12) {
      *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar7;
      bVar11 = false;
      uVar7 = candidate_aa760();
      if (bVar11) {
        *(undefined2 *)(unaff_A6 + 0x1c) = uVar7;
        candidate_aa83a();
        *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
        *unaff_A6 = candidate_34698;
      }
    }
    return;
  }
candidate_4188:
  candidate_4126();
  iVar6 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar6 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
code_r0x00034d60:
  candidate_35496();
  uVar12 = (ushort)unaff_D7 < 5;
  *(char *)((int)unaff_A6 + 0x8f) = -((ushort)unaff_D7 == 5);
  candidate_427a();
  candidate_1945c();
  if (!(bool)uVar12) {
    return;
  }
  uVar9 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
  if (0xfc10 < uVar9) {
    uVar9 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar2 = ~uVar9 & 0xfff;
    uVar9 = (ushort)CONCAT31(uVar8,-((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                                     '\x01' << (uVar2 & 7)) != 0));
    uRam002fffe0 = uVar9 & (ushort)CONCAT31(uVar10,-(0x7ff < uVar2));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar9 = (ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar9 ^ uVar4);
joined_r0x00034e06:
    if ((uVar9 & 1) != 0) {
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
  goto candidate_4188;
}
