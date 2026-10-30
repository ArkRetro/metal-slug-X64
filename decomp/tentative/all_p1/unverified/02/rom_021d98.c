/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x021d98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_21d98(void)

{
  ushort uVar1;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0x21dae;
  candidate_1436a();
  candidate_1ad8c();
  if (*(char *)((int)unaff_A6 + 0x81) == '\x01') {
    uVar2 = false;
    if ((*(char *)((int)unaff_A6 + 0x61) == '\0') && (candidate_22178(), (bool)uVar2)) {
      *unaff_A6 = candidate_21e24;
      uVar2 = 0;
    }
  }
  else {
    uVar2 = 0;
    if (*(char *)((int)unaff_A6 + 0x81) == '\0') {
      uVar1 = *(ushort *)(unaff_A6 + 0x1c);
      uVar2 = 0;
      if ((uVar1 != 0) &&
         (uVar2 = uVar1 < *(ushort *)(unaff_A5 + 0x699e),
         (short)uVar1 <= (short)*(ushort *)(unaff_A5 + 0x699e))) {
        *unaff_A6 = candidate_21f4e;
        uVar2 = 0;
      }
    }
  }
  candidate_427a();
  if ((bool)uVar2) {
    if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
      *unaff_A6 = candidate_21ad0;
    }
    else {
      *unaff_A6 = candidate_21f4e;
    }
  }
  candidate_220bc();
  return;
}
