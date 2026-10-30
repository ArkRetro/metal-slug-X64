/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x045608.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void thunk_FUN_0004560c(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *(char *)(unaff_A6 + 0x23) = (char)*psVar1;
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
