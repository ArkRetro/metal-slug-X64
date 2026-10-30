/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0320f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_320f8(void)

{
  char cVar1;
  char *pcVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  bVar3 = *(ushort *)(unaff_A6 + 0x1c) < *(ushort *)(unaff_A5 + 0x699e);
  if (bVar3) {
    candidate_4c24();
    if (bVar3) {
      pcVar2 = (char *)0x31d68;
      do {
        cVar1 = *pcVar2;
        if (cVar1 == '\0') goto LAB_00032130;
        pcVar2 = pcVar2 + 1;
      } while (cVar1 != *(char *)((int)unaff_A6 + 0x5a));
      *(undefined1 *)(unaff_A6[3] + 0x82) = 0xff;
      goto LAB_00032142;
    }
  }
LAB_00032130:
  if (*(char *)(unaff_A6[3] + 0x82) != '\0') {
    *unaff_A6 = candidate_32040;
  }
LAB_00032142:
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
