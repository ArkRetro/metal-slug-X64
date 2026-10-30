/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3a3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3a3c(void)

{
  undefined4 *unaff_A6;

  if (*(char *)((int)unaff_A6 + 0x7a) < '\x01') {
    candidate_18f92();
    *unaff_A6 = 0xd3a52;
  }
  if ((*(char *)((int)unaff_A6 + 0x61) < '\x02') &&
     (*(undefined1 *)(unaff_A6 + 0x18) = 1, *(char *)((int)unaff_A6 + 0x61) == '\0')) {
    *unaff_A6 = candidate_d3a7c;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7a) = 0;
  return;
}
