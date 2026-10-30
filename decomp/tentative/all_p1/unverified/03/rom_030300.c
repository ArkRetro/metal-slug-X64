/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030300.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x0003cfe8) */

void candidate_30300(void)

{
  byte bVar1;
  char cVar2;
  short sVar3;
  code *pcVar4;
  code *extraout_A1;
  int unaff_A6;

  bVar1 = *(byte *)(unaff_A6 + 0x8c);
  if ((bVar1 & 0xf) == 9) {
    cVar2 = (bVar1 & 0xf0) + 0x10;
  }
  else {
    cVar2 = bVar1 + 1;
  }
  *(char *)(unaff_A6 + 0x8c) = cVar2;
  sVar3 = 2;
  pcVar4 = candidate_3dd96;
  do {
    (*pcVar4)();
    sVar3 = sVar3 + -1;
    pcVar4 = extraout_A1;
  } while (sVar3 != 0);
  return;
}
