/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x095e08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_95e08(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  undefined2 uVar4;
  char extraout_D1b;
  ushort *extraout_A0;
  int extraout_A0_00;
  undefined4 uVar5;
  ushort *extraout_A0_01;
  ushort *extraout_A0_02;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  candidate_40bc();
  *(undefined2 *)(extraout_A0_00 + 0x70) = *(undefined2 *)(unaff_A6 + 0x1c);
  *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *(undefined1 *)(unaff_A6 + 0x25) = 0xff;
  candidate_4256();
  uVar5 = 0x95950;
  if (*(short *)(unaff_A6 + 0x1c) != 0) {
    uVar5 = 0x9595c;
  }
  unaff_A6[0x14] = uVar5;
  *unaff_A6 = 0x95e58;
  bVar6 = *(char *)((int)unaff_A6 + 0x95) == '\0';
  if ((!bVar6) && (candidate_95c76(), !bVar6)) {
code_r0x00095d2e:
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)(unaff_A6 + 0x25) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x96) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x97) = 0;
    *(undefined2 *)(unaff_A6 + 0x21) = 0xffff;
    *(undefined2 *)((int)unaff_A6 + 0x86) = 0xffff;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    unaff_A6[0x14] = 0;
    candidate_4256();
    *unaff_A6 = 0x95d6e;
    candidate_4c24();
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    unaff_A6[0xc] = 0;
    uVar7 = 0;
    candidate_1436a();
    candidate_1ad8c();
    candidate_427a();
    candidate_c8bec();
    if ((bool)uVar7) {
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    if (*(char *)((int)unaff_A6 + 0x95) != '\0') {
      candidate_95c38();
      return;
    }
    bVar6 = false;
    uVar4 = candidate_aa606();
    if (bVar6) {
      *(undefined2 *)(unaff_A6 + 0x21) = uVar4;
      bVar6 = false;
      uVar4 = candidate_95b64();
      if (bVar6) {
        *(undefined2 *)((int)unaff_A6 + 0x86) = uVar4;
        candidate_aa83a();
        candidate_ca2ce();
        *extraout_A0 = *extraout_A0 & 0xffef;
        *unaff_A6 = candidate_95e08;
      }
    }
    return;
  }
  if ((DAT_0010f000 & 0x80) == 0) {
    candidate_c05ec();
    bVar6 = false;
    if (extraout_D1b == '\0') {
      candidate_4c24();
      if (bVar6) {
        candidate_aa7fc();
        goto code_r0x00095d2e;
      }
    }
    else {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
  }
  candidate_ca2ce();
  if ((*extraout_A0_01 & 0x10) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x97) = 0xf;
    *(undefined1 *)((int)unaff_A6 + 0x93) = 1;
    if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x92) = 1;
      *(undefined2 *)(unaff_A6 + 0x22) = 0;
    }
    *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + 9;
  }
  if (*(short *)(unaff_A6 + 0x22) != 0) {
    *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
  }
  if (((*(char *)((int)unaff_A6 + 0x97) != '\0') &&
      (cVar3 = *(char *)((int)unaff_A6 + 0x97) + -1, *(char *)((int)unaff_A6 + 0x97) = cVar3,
      cVar3 == '\0')) && (*(undefined2 *)(unaff_A6 + 0x22) = 0, *(byte *)((int)unaff_A6 + 0x91) < 8)
     ) {
    *(undefined1 *)((int)unaff_A6 + 0x93) = 0xff;
  }
  if (*(char *)((int)unaff_A6 + 0x92) != '\0') {
    candidate_95b98();
  }
  if (((*(short *)(unaff_A6 + 0x22) == 0) && (*(char *)((int)unaff_A6 + 0x91) == '\0')) &&
     (*(char *)((int)unaff_A6 + 0x92) != '\0')) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
  }
  candidate_ca2ce();
  if ((*extraout_A0_02 & 0xffef) != 0) {
    candidate_95c76();
  }
  unaff_A6[0xc] = 0;
  uVar7 = 0;
  candidate_1436a();
  candidate_1ad8c();
  candidate_427a();
  candidate_95b64();
  if (!(bool)uVar7) {
    *unaff_A6 = candidate_95d2e;
  }
  return;
}
