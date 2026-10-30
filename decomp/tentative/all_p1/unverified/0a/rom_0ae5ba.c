/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae5ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ae5ba(void)

{
  char cVar1;
  int unaff_A6;

  cVar1 = *(char *)(unaff_A6 + 0x80) + *(char *)(unaff_A6 + 0x7e) * '\b';
  if (cVar1 != *(char *)(unaff_A6 + 0x95)) {
    if (cVar1 == '\x12') {
      candidate_ae244();
    }
    else if (*(char *)(unaff_A6 + 0x95) == '\x12') {
      candidate_ae244();
    }
    *(char *)(unaff_A6 + 0x95) = cVar1;
  }
  if (*(byte *)(unaff_A6 + 0x95) != 0x12) {
    *(undefined2 *)(unaff_A6 + 0x82) =
         *(undefined2 *)((short)((ushort)*(byte *)(unaff_A6 + 0x95) * 2) + 0xaacc0);
  }
  return;
}
