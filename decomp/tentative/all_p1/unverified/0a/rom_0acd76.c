/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0acd76.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_acd76(void)

{
  char cVar1;
  uint uVar2;
  ushort uVar3;
  short sVar4;
  int iVar5;
  ushort *extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  undefined4 *unaff_A6;
  bool bVar6;

  *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
  candidate_ae244();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined1 *)((int)unaff_A6 + 0x99) = 0xff;
  *unaff_A6 = 0xacd96;
  candidate_143ee();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_ae82e();
  }
  else {
    candidate_ae800();
  }
  candidate_ca2ce();
  sVar4 = *(short *)((int)unaff_A6 + 0x7e);
  uVar2 = candidate_ae51c();
  if ((uVar2 & 0x80) == 0) {
    if ((*(short *)((int)unaff_A6 + 0x7e) == 0) && (sVar4 != 0)) {
      candidate_40bc();
    }
    bVar6 = false;
    uVar3 = candidate_ae260();
    if (bVar6) {
      if (uVar3 == 0) {
        *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
        *(undefined1 *)((int)unaff_A6 + 0x99) = 0;
        unaff_A6[0xc] = 0;
        unaff_A6[0xd] = 0;
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        candidate_ae244();
        *unaff_A6 = 0xace64;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          unaff_A6[0xc] = 0;
          unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
          candidate_143ee();
        }
        else {
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
          candidate_14594();
        }
        candidate_1ad8c();
        candidate_427a();
        candidate_ae720();
        *unaff_A6 = candidate_ac870;
      }
      else {
        if (uVar3 != 2) {
          candidate_120f8();
          *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
          candidate_353c2();
          *(undefined1 *)((int)unaff_A6 + 0x99) = 0;
          unaff_A6[0xc] = 0;
          unaff_A6[0xd] = 0;
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
          candidate_ae244();
          *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
          *(undefined1 *)((int)unaff_A6 + 0x9d) = 0xff;
          *unaff_A6 = 0xaceec;
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            unaff_A6[0xc] = 0;
            unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
            func_0x000143e4();
          }
          else {
            unaff_A6[0xc] = 0;
            unaff_A6[0xd] = 0;
            func_0x000143e4();
          }
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            candidate_ae82e();
          }
          else {
            candidate_ae800();
          }
          bVar6 = false;
          candidate_ae748();
          if (!bVar6) {
            if ((*(char *)((int)unaff_A6 + 0x96) == '\0') &&
               (candidate_ca2ce(), (*extraout_A0 & 0x20) != 0)) {
              *(undefined1 *)((int)unaff_A6 + 0x96) = 0xff;
              candidate_ae244();
              candidate_ae720();
              candidate_411c();
              candidate_1acf2();
              candidate_40bc();
              candidate_13f1e();
            }
            candidate_1ad8c();
            candidate_427a();
            goto LAB_000ad1c4;
          }
        }
        if (*(char *)((int)unaff_A6 + 0x96) == '\0') {
          candidate_ae720();
        }
        candidate_411c();
        candidate_1acf2();
        *(undefined1 *)((int)unaff_A6 + 0x96) = 0xff;
        candidate_120f8();
        *unaff_A6 = candidate_41ac;
        candidate_1ad8c();
        candidate_427a();
        candidate_49e8();
      }
    }
    else {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        *unaff_A6 = candidate_accbe;
      }
      if ((uVar3 & 2) == 0) {
        *unaff_A6 = candidate_accbe;
      }
      candidate_ae5a0();
      candidate_ae86a();
      candidate_1ad8c();
      candidate_427a();
    }
  }
  else {
    candidate_120f8();
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x86) = 0x1e;
    *(undefined1 *)((int)unaff_A6 + 0x96) = 0xff;
    candidate_ae720();
    candidate_411c();
    candidate_1acf2();
    candidate_40bc();
    candidate_ae244();
    unaff_A6[0xd] = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    unaff_A6[0xc] = 0;
    candidate_13f1e();
    *unaff_A6 = 0xad08e;
    uVar2 = candidate_29c6();
    if ((uVar2 & 3) == 0) {
      candidate_59e98();
    }
    bVar6 = false;
    candidate_1495c();
    if (bVar6) {
      unaff_A6[0xd] = unaff_A6[0xd] + 0x800;
    }
    else {
      unaff_A6[0xd] = unaff_A6[0xd] + -0x800;
    }
    iVar5 = unaff_A6[0xc] + 0x12000;
    if (0x180000 < iVar5) {
      iVar5 = 0x180000;
    }
    unaff_A6[0xc] = iVar5;
    candidate_143ee();
    candidate_ae748();
    sVar4 = candidate_49e8();
    if (((sVar4 == 0) && (*(short *)(unaff_A6 + 0xf) < 0x111)) &&
       ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
      candidate_1ad8c();
      candidate_427a();
    }
    else {
      candidate_120f8();
      candidate_ae220();
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_00 + 0x30) = 0xfff30000;
      *(undefined4 *)(extraout_A0_00 + 0x34) = 0x180000;
      *(undefined2 *)(extraout_A0_00 + 0x4e) = 10;
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_01 + 0x30) = 0xb0000;
      *(undefined4 *)(extraout_A0_01 + 0x34) = 0x150000;
      *(undefined2 *)(extraout_A0_01 + 0x4e) = 7;
      candidate_ad9e6();
      *(undefined4 *)(extraout_A0_02 + 0x30) = 0x80000;
      *(undefined4 *)(extraout_A0_02 + 0x34) = 0xfffc0000;
      *(undefined2 *)(extraout_A0_02 + 0x4e) = 5;
      candidate_411c();
      candidate_1acf2();
      *unaff_A6 = candidate_41ac;
      candidate_1ad8c();
      candidate_427a();
      candidate_49e8();
    }
  }
LAB_000ad1c4:
  if (*(char *)((int)unaff_A6 + 0x96) == '\0') {
    bVar6 = false;
    if (*(short *)(unaff_A6 + 0x23) == 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
      candidate_ae8c2();
      if (bVar6) {
        *(undefined2 *)(unaff_A6 + 0x23) = 0x3c;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
        *(undefined2 *)((int)unaff_A6 + 0x8e) = 2;
        candidate_120f8();
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      candidate_ae8c2();
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      if (*(short *)((int)unaff_A6 + 0x8e) == 0) {
        sVar4 = *(short *)(unaff_A6 + 0x23) + -1;
        *(short *)(unaff_A6 + 0x23) = sVar4;
        if (sVar4 == 0) {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
        }
        if ((*(ushort *)(unaff_A6 + 0x23) & 2) == 0) {
          *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
        }
        else {
          *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
          *(undefined1 *)(unaff_A6 + 0x21) = 0xff;
        }
      }
      else {
        *(short *)((int)unaff_A6 + 0x8e) = *(short *)((int)unaff_A6 + 0x8e) + -1;
      }
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      candidate_ca2ce();
      if (*(char *)((int)unaff_A6 + 0x9d) == '\0') {
        candidate_ae610();
      }
      candidate_ae91a();
    }
    bVar6 = false;
    candidate_aa760();
    if (!bVar6) {
      *unaff_A6 = candidate_ac870;
    }
    candidate_140c0();
  }
  cVar1 = *(char *)(unaff_A6 + 0x26);
  *(char *)(unaff_A6 + 0x26) = cVar1 + -1;
  if (cVar1 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x26) = 0;
  }
  return;
}
