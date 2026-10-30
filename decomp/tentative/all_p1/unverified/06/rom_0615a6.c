/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0615a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x0006193e) */
/* WARNING: Removing unreachable block (ram,0x00061948) */
/* WARNING: Removing unreachable block (ram,0x0006194a) */
/* WARNING: Removing unreachable block (ram,0x00061962) */
/* WARNING: Removing unreachable block (ram,0x0006196a) */
/* WARNING: Removing unreachable block (ram,0x00061958) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_615a6(void)

{
  code *pcVar1;
  char cVar2;
  ushort uVar3;
  ushort uVar4;
  ushort extraout_D0u;
  ushort extraout_D0u_00;
  ushort uVar7;
  int iVar5;
  short sVar8;
  uint uVar6;
  short sVar9;
  undefined1 uVar10;
  short unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar11;
  bool bVar12;
  short sStack_6;
  undefined4 uStack_4;

  uVar10 = (undefined1)((ushort)unaff_D2w >> 8);
  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x20) == 0) {
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xf7;
    if ((*(byte *)((int)unaff_A6 + 0x96) & 4) == 0) {
      uStack_4 = 0x615c6;
      uVar7 = candidate_29c6();
      sVar8 = *(short *)((int)unaff_A6 + 0x8a) - (uVar7 & 7);
      if (sVar8 <= *(short *)((int)unaff_A6 + 0x8e)) {
        sVar8 = *(short *)((int)unaff_A6 + 0x8e) + 4;
      }
      *(short *)(unaff_A6 + 0x25) = sVar8;
      iVar5 = *(int *)((int)unaff_A6 + 0x72);
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        iVar5 = -iVar5;
      }
      *(int *)((int)unaff_A6 + 0x62) = iVar5;
      uStack_4 = 0x61606;
      candidate_4256();
      *unaff_A6 = 0x6160c;
      uVar11 = 0;
      uStack_4 = 0x61610;
      candidate_623da();
      uStack_4 = 0x61616;
      candidate_427a();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
        uStack_4 = 0x61624;
        sVar8 = candidate_62362();
        if ((bool)uVar11) {
          *unaff_A6 = candidate_61b38;
        }
        else if (sVar8 < 0) {
          *unaff_A6 = 0x6173c;
        }
        else {
          uStack_4 = 0x6164a;
          candidate_62338();
          uStack_4 = 0x6164e;
          candidate_6237c();
          uVar7 = *(ushort *)(unaff_A6 + 0x25);
          if ((short)uVar7 >= (short)extraout_D0u) {
            *unaff_A6 = 0x6173c;
          }
          bVar12 = (short)uVar7 < (short)extraout_D0u && extraout_D0u < uVar7;
          uStack_4 = 0x61662;
          candidate_625b0();
          if (bVar12) {
            *unaff_A6 = candidate_61ae4;
          }
        }
      }
      goto LAB_00061be8;
    }
    uStack_4 = 0x618b2;
    candidate_62362();
    if ((bool)in_CF) {
      *unaff_A6 = candidate_61b38;
      goto LAB_00061be8;
    }
    uStack_4 = 0x618c4;
    sVar8 = candidate_6243e();
    unaff_D2w = *(short *)((short)(sVar8 * 2) + 0x2d4d2c);
    if ((*(byte *)((int)unaff_A6 + 0x96) & 0x10) == 0) {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        unaff_D2w = -unaff_D2w;
      }
      unaff_D2w = *(short *)(unaff_A6 + 0xf) + unaff_D2w;
      uStack_4 = CONCAT22(unaff_D2w,uStack_4._2_2_);
      candidate_62338();
      register0x0000003c = (BADSPACEBASE *)&uStack_4;
      sVar9 = sStack_6 - *(short *)(extraout_A0 + 0x3c);
      if (sVar9 < 0) {
        sVar9 = -sVar9;
      }
      if (sVar9 <= *(short *)((int)unaff_A6 + 0x8e)) {
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x20;
        goto LAB_0006173c;
      }
    }
    uVar10 = (undefined1)((ushort)unaff_D2w >> 8);
    uVar7 = *(ushort *)((short)(sVar8 * 2) + 0x2d4d3c);
    *(ushort *)((int)unaff_A6 + 0x86) = (uVar7 >> 3) + (ushort)((uVar7 >> 2 & 1) != 0);
    *(undefined4 *)((int)register0x0000003c + -4) = 0x6199a;
    candidate_4256();
    *unaff_A6 = candidate_619a0;
    uVar11 = 0;
    *(undefined4 *)((int)register0x0000003c + -4) = 0x619a6;
    candidate_1436a();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x619ac;
    candidate_427a();
    if ((bool)uVar11) {
      *unaff_A6 = candidate_619ba;
    }
  }
  else {
LAB_0006173c:
    uVar10 = (undefined1)((ushort)unaff_D2w >> 8);
    *(undefined4 *)((int)register0x0000003c + -4) = 0x61742;
    uVar6 = candidate_29c6();
    *(char *)((int)unaff_A6 + 0x9a) = -((uVar6 & 0xf) == 0);
    *(undefined4 *)((int)register0x0000003c + -4) = 0x6175a;
    candidate_4256();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x61760;
    candidate_1b1dc();
    *unaff_A6 = 0x61766;
    uVar11 = 0;
    *(undefined4 *)((int)register0x0000003c + -4) = 0x6176a;
    candidate_623fe();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x61770;
    candidate_427a();
    *(undefined4 *)((int)register0x0000003c + -4) = 0x61774;
    candidate_62362();
    if ((bool)uVar11) {
      *unaff_A6 = candidate_61b38;
    }
    else {
      uVar11 = false;
      if (*(char *)((int)unaff_A6 + 0x9a) == '\0' && (*(byte *)((int)unaff_A6 + 0x96) & 0x21) == 0)
      {
        *(undefined4 *)((int)register0x0000003c + -4) = 0x61796;
        candidate_62338();
        *(undefined4 *)((int)register0x0000003c + -4) = 0x6179a;
        candidate_6237c();
        if (((*(byte *)((int)unaff_A6 + 0x96) & 8) == 0) &&
           ((short)extraout_D0u_00 <= *(short *)((int)unaff_A6 + 0x8e))) {
          *unaff_A6 = candidate_61670;
        }
        uVar11 = extraout_D0u_00 < *(ushort *)((int)unaff_A6 + 0x8a);
        if ((short)*(ushort *)((int)unaff_A6 + 0x8a) < (short)extraout_D0u_00) {
          *unaff_A6 = candidate_615a6;
          uVar11 = false;
        }
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0x617c6;
      candidate_6238c();
      if ((bool)uVar11) {
        *unaff_A6 = candidate_61808;
        uVar11 = 0;
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0x617d4;
      candidate_625b0();
      if ((bool)uVar11) {
        *unaff_A6 = candidate_61ae4;
      }
      else if (*(short *)(unaff_A6 + 0x21) < 1) {
        *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
        if ((*(byte *)((int)unaff_A6 + 0x96) & 8) != 0) {
          *unaff_A6 = candidate_61848;
        }
      }
    }
  }
LAB_00061be8:
  bVar12 = false;
  if (0 < *(short *)(unaff_A6 + 0x21)) {
    sVar8 = *(short *)(unaff_A6 + 0x21);
    bVar12 = sVar8 == 0;
    *(short *)(unaff_A6 + 0x21) = sVar8 + -1;
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0x61c02;
  candidate_1945c();
  if (!bVar12) {
    bVar12 = false;
    if ('\0' < *(char *)((int)unaff_A6 + 0x9d)) {
      cVar2 = *(char *)((int)unaff_A6 + 0x9d);
      bVar12 = cVar2 == '\0';
      *(char *)((int)unaff_A6 + 0x9d) = cVar2 + -1;
    }
    *(undefined4 *)((int)register0x0000003c + -4) = 0x61c18;
    candidate_4c24();
    if (bVar12) {
      *(undefined4 *)((int)register0x0000003c + -4) = 0x61c20;
      candidate_625ec();
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      cVar2 = *(char *)((int)unaff_A6 + 0x5a);
      *unaff_A6 = candidate_61c6e;
      if (cVar2 == '\n') {
        *unaff_A6 = candidate_61e16;
      }
      if (cVar2 == '\v') {
        *unaff_A6 = candidate_61e2e;
      }
      if (cVar2 == '\a') {
        *unaff_A6 = candidate_61d46;
      }
      *(undefined4 *)((int)register0x0000003c + -4) = 0x61c68;
      candidate_4e76();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    return;
  }
  uVar7 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
  if (0xfc10 < uVar7) {
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar4 = uRam002fffe8;
    uVar3 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11(uVar10,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                              '\x01' << (uVar3 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4) & 1) != 0) {
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
      *(undefined4 *)((int)register0x0000003c + -4) = 0x3d2c;
      func_0x0003cf12();
      *(undefined4 *)((int)register0x0000003c + -4) = 0x3d36;
      candidate_3e2a8();
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  }
  *(undefined4 *)((int)register0x0000003c + -4) = 0x418a;
  candidate_4126();
  iVar5 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar5 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
