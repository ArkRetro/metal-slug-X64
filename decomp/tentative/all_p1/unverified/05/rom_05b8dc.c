/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05b8dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5b8dc(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_5c07c();
  candidate_21288();
  if ((bool)in_CF) {
    *unaff_A6 = 0x5ba32;
    in_CF = 0;
  }
  if (((*(byte *)(unaff_A6 + 0x21) & 0x80) == 0) && (in_CF = 0, *(char *)(unaff_A6 + 0x20) == '\0'))
  {
    *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) | 0x80;
    candidate_5c0ca();
  }
  candidate_5c0fa();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x7e) = 0x10;
    in_CF = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_5b950;
    in_CF = 0;
  }
  candidate_1945c();
  if ((bool)in_CF) {
    *unaff_A6 = 0x5bb1a;
  }
  return;
}
