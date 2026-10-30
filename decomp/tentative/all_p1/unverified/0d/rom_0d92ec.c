/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d92ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d92ec(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  cVar1 = *(char *)(unaff_A6[3] + (int)(short)((*(byte *)((int)unaff_A6 + 0x89) & 7) + 0x84));
  if (cVar1 != *(char *)(unaff_A6 + 0x21)) {
    *(char *)(unaff_A6 + 0x21) = cVar1;
    *(char *)((int)unaff_A6 + 0x85) = cVar1;
    *unaff_A6 = candidate_d819a;
    if ((*(byte *)(unaff_A6 + 0x21) & 0x80) != 0) {
      *unaff_A6 = *(undefined4 *)((short)((*(byte *)(unaff_A6 + 0x21) & 0x7f) << 2) + 0xe9bc0);
    }
  }
  return;
}
