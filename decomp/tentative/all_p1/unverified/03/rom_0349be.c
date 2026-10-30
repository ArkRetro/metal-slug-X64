/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0349be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_349be(void)

{
  code *pcVar1;
  ushort uVar2;
  short sVar3;
  undefined2 uVar5;
  uint uVar4;
  int iVar6;
  ushort *extraout_A0;
  ushort *extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar7;
  bool bVar8;

  unaff_A6[0xd] = 0x8f5c2;
  candidate_350e4();
  unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x62);
  *unaff_A6 = 0x349d8;
  candidate_ca2ce();
  iVar6 = unaff_A6[0xc];
  if ((*extraout_A0 & 8) == 0) {
    if (((*extraout_A0 & 4) != 0) && (iVar6 = iVar6 + -0x9555, iVar6 < -0x38000)) {
      iVar6 = -0x38000;
    }
  }
  else {
    iVar6 = iVar6 + 0x9555;
    if (0x38000 < iVar6) {
      iVar6 = 0x38000;
    }
  }
  unaff_A6[0xc] = iVar6;
  unaff_A6[0xd] = unaff_A6[0xd] + -0xb780;
  candidate_35100();
  candidate_3547e();
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) || ((short)((uint)iVar6 >> 0x10) != 5)) {
    if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) && ((int)unaff_A6[0xd] < 1)) {
      *unaff_A6 = candidate_34786;
      candidate_ca2ce();
      uVar2 = *extraout_A0_00;
      if ((uVar2 & 8) != 0) {
        *unaff_A6 = candidate_348d8;
      }
      if ((uVar2 & 4) != 0) {
        *unaff_A6 = candidate_348d8;
      }
      if ((uVar2 & 2) != 0) {
        *unaff_A6 = candidate_34786;
      }
    }
    candidate_ca2ce();
    bVar8 = false;
    candidate_3517c();
    if (bVar8) {
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
      uVar7 = 0;
      candidate_427a();
      candidate_c8bec();
      if (!(bool)uVar7) {
        uVar5 = candidate_aa610();
        if ((bool)uVar7) {
          *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar5;
          bVar8 = false;
          uVar5 = candidate_aa760();
          if (bVar8) {
            *(undefined2 *)(unaff_A6 + 0x1c) = uVar5;
            candidate_aa83a();
            *(undefined2 *)(unaff_A6 + 0x20) = 0x1e;
            *unaff_A6 = candidate_34698;
          }
        }
        return;
      }
      candidate_4126();
      iVar6 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar6 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
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
    uVar4 = candidate_29c6();
    *(char *)((int)unaff_A6 + 0x8b) = -((uVar4 & 3) == 0);
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
      uVar2 = *(short *)(unaff_A6 + 0x1f) + 1;
      *(ushort *)(unaff_A6 + 0x1f) = uVar2;
      if (0x1e < uVar2) {
        *unaff_A6 = 0x34aca;
      }
      if ((9 < uVar2) && (uVar4 = candidate_29c6(), (uVar4 & 0xf) == 0)) {
        *unaff_A6 = 0x34aca;
      }
    }
    if ((*(char *)((int)unaff_A6 + 0x8e) == '\0') && (*(char *)((int)unaff_A6 + 0x8d) != '\0')) {
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
    bVar8 = false;
    candidate_aa760();
    if (!bVar8) {
      *unaff_A6 = 0x345a0;
    }
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + 1;
  }
  return;
}
