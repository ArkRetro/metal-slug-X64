/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ca12.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8ca12(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  cVar1 = *(char *)((int)unaff_A6 + 0x82) + -1;
  *(char *)((int)unaff_A6 + 0x82) = cVar1;
  if (cVar1 == '\0') {
    *unaff_A6 = candidate_8ca24;
  }
  if ((*(byte *)((int)unaff_A6 + 0x85) & 2) == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  candidate_1ac38();
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x01';
  return;
}
