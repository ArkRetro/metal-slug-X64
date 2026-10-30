/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03610e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3610e(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
    candidate_427a();
    func_0x0003623e();
    return;
  }
  *(undefined1 *)((int)unaff_A6 + 0x9b) = 0xff;
  cVar1 = candidate_29f2();
  if (cVar1 == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x9b) = 8;
    *(undefined1 *)(unaff_A6 + 0x27) = 0;
  }
  if (*(char *)((int)unaff_A6 + 0x9f) == '\a') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = candidate_3618a;
  return;
}
