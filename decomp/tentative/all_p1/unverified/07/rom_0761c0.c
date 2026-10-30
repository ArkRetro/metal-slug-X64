/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0761c0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_761c0(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    if (*(char *)(unaff_A6 + 0x1c) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
  }
  else if (*(char *)(unaff_A6 + 0x1c) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x7622c;
  uVar2 = 0;
  candidate_768ae();
  candidate_7693a();
  candidate_768f4();
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_7626e;
    if ((*(char *)(unaff_A6[3] + 0x85) == -1) && (*(int *)(unaff_A6[3] + 0x50) == 0)) {
      *unaff_A6 = candidate_1c706;
    }
  }
  bVar1 = *(char *)(unaff_A6[3] + 0x60) != -1;
  if (!bVar1) {
    *unaff_A6 = 0x765d4;
  }
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = 0x765d4;
  }
  return;
}
