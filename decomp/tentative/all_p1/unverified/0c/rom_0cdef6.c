/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cdef6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cdef6(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -0x1e;
  candidate_4256();
  *unaff_A6 = 0xcdf12;
  uVar1 = 0;
  candidate_ce6c0();
  candidate_427a();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *unaff_A6 = 0xcdde0;
  }
  if (*(char *)(unaff_A6[3] + 0xa0) == '\x02') {
    *unaff_A6 = candidate_4188;
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cdf54;
  }
  return;
}
