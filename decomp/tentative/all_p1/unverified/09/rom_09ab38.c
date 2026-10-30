/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ab38.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ab38(void)

{
  char in_D0b;
  ushort *puVar1;
  char *pcVar2;
  undefined1 in_CF;

  candidate_1aeb0();
  if ((bool)in_CF) {
    if (in_D0b == '\0') {
      puVar1 = (ushort *)&DAT_0010c5fc;
      pcVar2 = &DAT_0010c5fe;
    }
    else {
      puVar1 = (ushort *)&DAT_0010c600;
      pcVar2 = &DAT_0010c602;
    }
    *puVar1 = *puVar1 + 1;
    if (99 < *puVar1) {
      *puVar1 = 99;
    }
    *pcVar2 = *pcVar2 + '\x01';
  }
  return;
}
