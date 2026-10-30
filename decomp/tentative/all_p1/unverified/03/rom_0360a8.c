/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0360a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_360a8(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  if (!(bool)in_CF) {
    func_0x0003623e();
    return;
  }
  if (*(char *)((int)unaff_A6 + 0x9f) == '\a') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  if (*(char *)((int)unaff_A6 + 0x96) == '\x03') {
    *(char *)(unaff_A6[3] + 0x99) = *(char *)(unaff_A6[3] + 0x99) + '\x01';
  }
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *unaff_A6 = candidate_3610e;
  return;
}
