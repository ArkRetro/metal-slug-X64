/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ddc90.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ddc90(void)

{
  short sVar1;
  short sVar2;
  char cVar3;
  short in_D1w;
  int in_A0;
  int unaff_A6;

  if ((*(int *)(unaff_A6 + 0x7e) != 0) &&
     (sVar1 = *(short *)(in_A0 + 0x40) - in_D1w,
     sVar2 = *(short *)(*(int *)(unaff_A6 + 0x7e) + 0x40) - in_D1w, sVar1 <= sVar2)) {
    if (sVar2 == sVar1) {
      cVar3 = candidate_29c6();
      if (cVar3 < '\0') goto LAB_000ddcc2;
    }
    return;
  }
LAB_000ddcc2:
  *(int *)(unaff_A6 + 0x7e) = in_A0;
  *(char *)(unaff_A6 + 0xa3) = -(*(short *)(in_A0 + 0x3c) <= *(short *)(unaff_A6 + 0x98));
  return;
}
