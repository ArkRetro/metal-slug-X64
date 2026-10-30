/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03959c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3959c(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_39d74();
  uVar1 = *(ushort *)(unaff_A5 + 0x699e) < *(ushort *)(unaff_A6 + 0x1d);
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_3965c;
  }
  candidate_39b4a();
  candidate_4c24();
  if ((bool)uVar1) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      *unaff_A6 = candidate_39820;
    }
  }
  if (*(char *)((int)unaff_A6 + 0x93) != '\0') {
    *(char *)((int)unaff_A6 + 0x93) = *(char *)((int)unaff_A6 + 0x93) + -1;
  }
  return;
}
