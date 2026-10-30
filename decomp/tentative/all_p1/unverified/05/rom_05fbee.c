/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05fbee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5fbee(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  if ((((*(char *)((int)unaff_A6 + 0x8a) != '\0') &&
       (sVar1 = *(short *)((int)unaff_A6 + 0x7e), *(short *)((int)unaff_A6 + 0x7e) = sVar1 + -1,
       sVar1 == 0)) &&
      (*(undefined2 *)((int)unaff_A6 + 0x7e) = 0, *(char *)((int)unaff_A6 + 0x89) == '\0')) &&
     (bVar2 = false, *(char *)((int)unaff_A6 + 0x8d) == '\0')) {
    candidate_5fc2c();
    if (bVar2) {
      *unaff_A6 = candidate_5f486;
    }
    else {
      *unaff_A6 = candidate_5f5f0;
    }
  }
  return;
}
