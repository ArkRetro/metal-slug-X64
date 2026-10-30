/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a0664.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a0664(void)

{
  byte bVar1;
  char cVar2;
  int unaff_A6;
  undefined1 in_CF;

  candidate_a0630();
  if ((bool)in_CF) {
    return;
  }
  candidate_a059a();
  if (!(bool)in_CF) {
    return;
  }
  candidate_1aeda();
  cVar2 = candidate_aaa88();
  if (cVar2 == '\0') {
    if ((*(byte *)(unaff_A6 + 0x8c) & 2) == 0) {
      bVar1 = *(byte *)(unaff_A6 + 0x90);
joined_r0x000a06c2:
      if ((bVar1 & 2) == 0) {
        return;
      }
      return;
    }
    bVar1 = *(byte *)(unaff_A6 + 0x90);
  }
  else {
    if ((*(byte *)(unaff_A6 + 0x8c) & 1) != 0) {
      bVar1 = *(byte *)(unaff_A6 + 0x90);
      goto joined_r0x000a06c2;
    }
    bVar1 = *(byte *)(unaff_A6 + 0x90);
  }
  if ((bVar1 & 2) != 0) {
    return;
  }
  return;
}
