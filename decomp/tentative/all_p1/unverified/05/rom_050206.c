/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x050206.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_50206(void)

{
  byte bVar1;
  undefined1 unaff_D7b;
  int unaff_A6;
  char cVar2;

  candidate_50296();
  *(undefined1 *)(unaff_A6 + 0x7d) = unaff_D7b;
  candidate_50296();
  *(undefined1 *)(unaff_A6 + 0x7e) = unaff_D7b;
  cVar2 = *(char *)(unaff_A6 + 0x7d) != '\0';
  if (*(char *)(unaff_A6 + 0x7e) != '\0') {
    cVar2 = cVar2 + '\x01';
  }
  bVar1 = *(char *)(unaff_A6 + 0x7e) + *(char *)(unaff_A6 + 0x7d);
  if (cVar2 == '\x02') {
    bVar1 = bVar1 >> 1;
  }
  if ((*(char *)(unaff_A6 + 0x7c) == '\0') && (3 < bVar1)) {
    *(undefined1 *)(unaff_A6 + 0x7c) = 5;
  }
  else {
    if ((*(char *)(unaff_A6 + 0x7c) != '\0') && (bVar1 == 0)) {
      *(undefined1 *)(unaff_A6 + 0x7c) = 0;
      return;
    }
    if (bVar1 != *(byte *)(unaff_A6 + 0x7c)) {
      if (*(byte *)(unaff_A6 + 0x7c) < bVar1) {
        *(char *)(unaff_A6 + 0x7c) = *(char *)(unaff_A6 + 0x7c) + '\x01';
      }
      else {
        *(char *)(unaff_A6 + 0x7c) = *(char *)(unaff_A6 + 0x7c) + -1;
      }
    }
  }
  return;
}
