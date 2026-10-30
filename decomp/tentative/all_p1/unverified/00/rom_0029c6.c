/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0029c6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29c6(void)

{
  ushort uVar1;
  ushort *puVar2;
  ushort *puVar3;

  puVar2 = puRam00100084 + 1;
  uVar1 = *puRam00100084;
  if ((ushort *)0x100083 < puVar2) {
    puVar2 = puRam00100084 + -0x36;
  }
  puVar3 = puVar2 + 0x17;
  if ((ushort *)0x100083 < puVar3) {
    puVar3 = puVar2 + -0x20;
  }
  puRam00100084 = puVar2;
  *puVar3 = uVar1 ^ *puVar3;
  return;
}
