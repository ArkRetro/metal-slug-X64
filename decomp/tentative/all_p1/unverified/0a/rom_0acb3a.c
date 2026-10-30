/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0acb3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_acb3a(void)

{
  char cVar1;
  int iVar2;
  short sVar3;
  ushort *extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_411c();
  candidate_1acf2();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
  candidate_ae244();
  *(undefined1 *)((int)unaff_A6 + 0x95) = 0x12;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x280;
  *(undefined2 *)(unaff_A6 + 0x20) = 0x280;
  unaff_A6[0x22] = 0x100000;
  candidate_120f8();
  *unaff_A6 = 0xacb96;
  candidate_143ee();
  candidate_ca2ce();
  candidate_ae2d2();
  bVar4 = false;
  sVar3 = candidate_ae260();
  if (bVar4) {
    if (sVar3 == 0) {
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
      if (sVar3 != 2) {
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
        bVar4 = false;
        candidate_ae748();
        if (!bVar4) {
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
    iVar2 = ((int)unaff_A6[0x22] >> 2) + ((int)unaff_A6[0x22] >> 1);
    unaff_A6[0xd] = iVar2 + unaff_A6[0xd];
    unaff_A6[0x22] = iVar2;
    if (iVar2 < 0x1000) {
      *unaff_A6 = 0xacc0a;
    }
    candidate_ae5ba();
    candidate_ae86a();
    candidate_1ad8c();
    candidate_427a();
  }
LAB_000ad1c4:
  if (*(char *)((int)unaff_A6 + 0x96) == '\0') {
    bVar4 = false;
    if (*(short *)(unaff_A6 + 0x23) == 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
      candidate_ae8c2();
      if (bVar4) {
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
        sVar3 = *(short *)(unaff_A6 + 0x23) + -1;
        *(short *)(unaff_A6 + 0x23) = sVar3;
        if (sVar3 == 0) {
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
    bVar4 = false;
    candidate_aa760();
    if (!bVar4) {
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
