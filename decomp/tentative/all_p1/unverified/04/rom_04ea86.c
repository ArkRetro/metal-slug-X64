/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ea86.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ea86(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4fade();
  candidate_1ac38();
  candidate_1ad8c();
  candidate_427a();
  uVar3 = false;
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    candidate_1af9a();
    if (!(bool)uVar3) {
      sVar1 = *(short *)((int)unaff_A6 + 0x72);
      uVar3 = sVar1 == 0;
      sVar1 = sVar1 + -1;
      *(short *)((int)unaff_A6 + 0x72) = sVar1;
      if (sVar1 == 0) {
        uVar2 = candidate_260c8();
        *(undefined2 *)((int)unaff_A6 + 0x72) = uVar2;
        candidate_40bc();
        candidate_40bc();
        candidate_1acf2();
        candidate_40bc();
        uVar3 = 0;
        candidate_1acf2();
      }
    }
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4eb36;
  }
  candidate_4f7e8();
  return;
}
