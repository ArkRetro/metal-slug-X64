/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x095d2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_95d2e(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  ushort *extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

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
  uVar4 = 0;
  candidate_1436a();
  candidate_1ad8c();
  candidate_427a();
  candidate_c8bec();
  if ((bool)uVar4) {
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
  bVar5 = false;
  uVar3 = candidate_aa606();
  if (bVar5) {
    *(undefined2 *)(unaff_A6 + 0x21) = uVar3;
    bVar5 = false;
    uVar3 = candidate_95b64();
    if (bVar5) {
      *(undefined2 *)((int)unaff_A6 + 0x86) = uVar3;
      candidate_aa83a();
      candidate_ca2ce();
      *extraout_A0 = *extraout_A0 & 0xffef;
      *unaff_A6 = candidate_95e08;
    }
  }
  return;
}
