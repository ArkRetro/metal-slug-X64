/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06ed5e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6ed5e(void)

{
  char cVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  if (*(char *)(unaff_A6 + 0x7d) == '\0') {
    return;
  }
  if (*(char *)(unaff_A6 + 0x7e) != '\0') {
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x7d) = 0;
  bVar2 = false;
  candidate_c07f8();
  if (!bVar2) {
    return;
  }
  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)(unaff_A6 + 0x70);
  *(undefined1 *)(unaff_A6 + 0x7e) = 0x1e;
  cVar1 = func_0x000c07e0();
  if (cVar1 != '\0') {
    candidate_c0808();
    candidate_1ac94();
  }
  return;
}
