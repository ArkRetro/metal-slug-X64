/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca5b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ca5b0(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;

  sVar1 = 4;
  puVar2 = (undefined1 *)0xca1b0;
  puVar3 = (undefined1 *)0x10f07c;
  puVar4 = (undefined1 *)0x10f080;
  do {
    *puVar3 = *puVar2;
    *puVar4 = *puVar2;
    sVar1 = sVar1 + -1;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    puVar4 = puVar4 + 1;
  } while (sVar1 != 0);
  candidate_ca5ee();
  candidate_ca5ee();
  return;
}
